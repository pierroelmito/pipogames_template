#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include <raylib.h>

namespace pgce {

template <typename E, E, typename T>
struct CmpMeta {
	static inline consteval bool has() { return false; }
};

#define DEF(CLASS, CMP, M)                                           \
	template <>                                                      \
	struct pgce::CmpMeta<decltype(CMP), CMP, CLASS> {                \
		using Ret = decltype(CLASS::M);                              \
		static inline Ret& get(CLASS& e) { return e.M; }             \
		static inline const Ret& get(const CLASS& e) { return e.M; } \
		static inline consteval bool has() { return true; }          \
	}

template <typename E>
struct M {
	template <E... cmps>
	struct H {
		template <typename T>
		consteval static bool has()
		{
			return (CmpMeta<decltype(cmps), cmps, T>::has() && ...);
		}
		// ===============================================================
		// entity
		template <typename T, typename F>
		static bool visit(T& e, F&& fn)
		{
			if constexpr (has<T>())
				fn(CmpMeta<decltype(cmps), cmps, T>::get(e)...);
			return true;
		}
		template <typename T, typename F>
		static bool visit(const T& e, F&& fn)
		{
			if constexpr (has<T>())
				fn(CmpMeta<decltype(cmps), cmps, T>::get(e)...);
			return true;
		}
		// optional
		template <typename T, typename F>
		static bool visit(std::optional<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				if (e)
					visit(*e, fn);
			return true;
		}
		template <typename T, typename F>
		static bool visit(const std::optional<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				if (e)
					visit(*e, fn);
			return true;
		}
		// vector
		template <typename T, typename F>
		static bool visit(std::vector<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				for (auto& i : e)
					visit(i, fn);
			return true;
		}
		template <typename T, typename F>
		static bool visit(const std::vector<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				for (const auto& i : e)
					visit(i, fn);
			return true;
		}
		// array
		template <typename T, size_t N, typename F>
		static bool visit(std::array<T, N>& e, F&& fn)
		{
			if constexpr (has<T>())
				for (auto& i : e)
					visit(i, fn);
			return true;
		}
		template <typename T, size_t N, typename F>
		static bool visit(const std::array<T, N>& e, F&& fn)
		{
			if constexpr (has<T>())
				for (const auto& i : e)
					visit(i, fn);
			return true;
		}
		// tuple
		template <typename F, typename... ES>
		static bool visit(std::tuple<ES...>& db, F&& f)
		{
			return (visit(std::get<ES>(db), f), ...);
		}
		template <typename F, typename... ES>
		static bool visit(const std::tuple<ES...>& db, F&& f)
		{
			return (visit(std::get<ES>(db), f), ...);
		}
		// ===============================================================
		// entity
		template <typename T, typename F>
		static bool remove(T& e, F&& fn)
		{
			if constexpr (has<T>())
				return fn(CmpMeta<decltype(cmps), cmps, T>::get(e)...);
			return false;
		}
		// optional
		template <typename T, typename F>
		static bool remove(std::optional<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				if (e && remove(*e, fn))
					e = std::nullopt;
			return false;
		}
		// vector
		template <typename T, typename F>
		static bool remove(std::vector<T>& e, F&& fn)
		{
			if constexpr (has<T>())
				e.erase(std::remove_if(e.begin(), e.end(), [&](T& i) { return remove(i, fn); }), e.end());
			return false;
		}
		// array
		template <typename T, size_t N, typename F>
		static bool remove(std::array<T, N>& e, F&& fn)
		{
			if constexpr (has<T>())
				for (auto& i : e)
					remove(i, fn);
			return false;
		}
		// tuple
		template <typename F, typename... ES>
		static bool remove(std::tuple<ES...>& db, F&& f)
		{
			return (remove(std::get<ES>(db), f), ...);
		}
	};
};

template <typename T, typename C>
struct Maker {
	static inline bool add(C& c, T&& e)
	{
		return true;
	}
};

template <typename T>
struct Maker<T, std::vector<T>> {
	static inline bool add(std::vector<T>& c, T&& e)
	{
		c.emplace_back() = std::move(e);
		return false;
	}
};

template <typename T>
struct Maker<T, std::optional<T>> {
	static inline bool add(std::optional<T>& c, T&& e)
	{
		c.emplace() = std::move(e);
		return false;
	}
};

struct A {
	template <typename T, typename DB, size_t... Is>
	static bool make(DB& db, T&& e, std::index_sequence<Is...> ints = {})
	{
		if constexpr (std::tuple_size_v<DB> != sizeof...(Is)) {
			return make(db, std::move(e), std::make_index_sequence<std::tuple_size_v<DB>> {});
		} else {
			return (Maker<T, std::tuple_element_t<Is, DB>>::add(std::get<Is>(db), std::move(e)) && ...);
		}
	}
};

} // namespace pgce
