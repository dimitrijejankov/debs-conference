#pragma once

#include <algorithm>
#include <cstdint>
#include <list>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <unordered_map>

namespace lru {

// small class used to store the cache results
    struct cache_result {
        circular_queue *w;
        double prob;
        bool is_anomaly;
    };

/**
 * base class to prevent copy
 * use as ClassName : private NoCopy {}
 * to prevent copy constructor of ClassName and assignment by copy
 */
    class NoCopy {
    public:
        virtual ~NoCopy() = default;

    protected:
        NoCopy() = default;

    private:
        NoCopy(const NoCopy &) = delete;

        const NoCopy &operator=(const NoCopy &) = delete;
    };

/*
 * a noop lockable concept that can be used in place of std::mutex
 */
    class NullLock {
    public:
        void lock() {}

        void unlock() {}

        bool try_lock() { return true; }
    };

/**
 * error raised when a key not in cache is passed to get()
 */
    class KeyNotFound : public std::invalid_argument {
    public:
        KeyNotFound() : std::invalid_argument("key_not_found") {}
    };

    struct KeyValuePair {
    public:
        uint64_t key;
        cache_result value;

        KeyValuePair(const uint64_t &k, const cache_result &v) : key(k), value(v) {}
    };

    typedef std::unordered_map<uint64_t, typename std::list<KeyValuePair>::iterator> Map;

/**
 *	The LRU Cache class templated by
 *		Key - key type
 *		Value - value type
 *		MapType - an associative container like std::unordered_map
 *		LockType - a lock type derived from the Lock class (default:
 *NullLock = no synchronization)
 *
 *	The default NullLock based template is not thread-safe, however passing
 *Lock=std::mutex will make it
 *	thread-safe
 */
    class cache : private NoCopy {
    public:
        typedef KeyValuePair node_type;
        typedef std::list<KeyValuePair> list_type;
        typedef Map map_type;
        typedef NullLock lock_type;
        using Guard = std::lock_guard<lock_type>;

        /**
         * the max size is the hard limit of keys and (maxSize + elasticity) is the
         * soft limit
         * the cache is allowed to grow till maxSize + elasticity and is pruned back
         * to maxSize keys
         * set maxSize = 0 for an unbounded cache (but in that case, you're better off
         * using a std::unordered_map
         * directly anyway! :)
         */
        explicit cache(size_t maxSize = 300000, size_t elasticity = 1000)
                : maxSize_(maxSize), elasticity_(elasticity) {}

        ~cache() {
            for(auto it : keys_) {
                delete it.value.w;
            }
        };

        size_t size() const {
            Guard g(lock_);
            return cache_.size();
        }

        bool empty() const {
            Guard g(lock_);
            return cache_.empty();
        }

        void clear() {
            Guard g(lock_);
            cache_.clear();
            keys_.clear();
        }

        void insert(const uint64_t &k, const cache_result &v) {
            Guard g(lock_);
            const auto iter = cache_.find(k);
            if (iter != cache_.end()) {
                delete iter->second->value.w;

                iter->second->value.w = v.w;
                iter->second->value.is_anomaly = v.is_anomaly;
                iter->second->value.prob = v.prob;

                keys_.splice(keys_.begin(), keys_, iter->second);
                return;
            }

            keys_.emplace_front(k, v);
            cache_[k] = keys_.begin();
            prune();
        }

        bool tryGet(const uint64_t &kIn, cache_result &vOut) {
            Guard g(lock_);
            const auto iter = cache_.find(kIn);
            if (iter == cache_.end()) {
                return false;
            }
            keys_.splice(keys_.begin(), keys_, iter->second);
            vOut = iter->second->value;
            return true;
        }

        const cache_result &get(const uint64_t &k) {
            Guard g(lock_);
            const auto iter = cache_.find(k);
            if (iter == cache_.end()) {
                throw KeyNotFound();
            }
            keys_.splice(keys_.begin(), keys_, iter->second);
            return iter->second->value;
        }

        bool remove(const uint64_t &k) {
            Guard g(lock_);
            auto iter = cache_.find(k);
            if (iter == cache_.end()) {
                return false;
            }

            delete iter->second->value.w;
            keys_.erase(iter->second);
            cache_.erase(iter);
            return true;
        }

        bool contains(const uint64_t &k) {
            Guard g(lock_);
            return cache_.find(k) != cache_.end();
        }

        size_t getMaxSize() const { return maxSize_; }

        size_t getElasticity() const { return elasticity_; }

        size_t getMaxAllowedSize() const { return maxSize_ + elasticity_; }

        template<typename F>
        void cwalk(F &f) const {
            Guard g(lock_);
            std::for_each(keys_.begin(), keys_.end(), f);
        }

    protected:
        size_t prune() {
            size_t maxAllowed = maxSize_ + elasticity_;
            if (maxSize_ == 0 || cache_.size() < maxAllowed) {
                return 0;
            }
            size_t count = 0;
            while (cache_.size() > maxSize_) {
                delete keys_.back().value.w;
                cache_.erase(keys_.back().key);
                keys_.pop_back();
                ++count;
            }
            return count;
        }

    private:
        mutable NullLock lock_;
        Map cache_;
        list_type keys_;
        size_t maxSize_;
        size_t elasticity_;
    };

}  // namespace LRUCache11
