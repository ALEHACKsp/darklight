#pragma once
// used: std::any
#include <any>
// used: std::filesystem
#include <filesystem>
// used: std::deque
#include <deque>
// used: std::vector
#include <vector>

// used: winapi includes
#include "../common.h"
// used: color
#include "../sdk/datatypes/color.h"
// used: fnv1a hashing for variables
#include "../sdk/hash/fnv1a.h"
#include "../global.h"

#define C_ADDVARIABLE( Type, szName, pDefault ) const std::uint32_t szName = C::AddVariable<Type>(FNV1A::HashConst(#szName), FNV1A::HashConst(#Type), pDefault);
#define C_ADDVARIABLE_VECTOR( Type, uSize, szName, pDefault ) const std::uint32_t szName = C::AddVariable<std::deque<Type>>(FNV1A::HashConst(#szName), FNV1A::HashConst("std::deque<" #Type ">"), MEM::GetFilledVector<Type, uSize>(pDefault));
#define C_INVALID_VARIABLE (std::size_t)-1

struct VariableObject_t
{
	VariableObject_t() = default;

	explicit VariableObject_t(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, const std::any pDefault)
		: uNameHash(uNameHash), uTypeHash(uTypeHash), pValue(pDefault) { }

	~VariableObject_t() = default;

	/* get casted variable value */
	template<typename T>
	T& Get()
	{
		return *(T*)(std::any_cast<T>(&pValue));
	}

	/* emplace casted variable value */
	template<typename T>
	void Set(T value)
	{
		pValue.emplace<T>(value);
	}

	FNV1A_t		uNameHash;
	FNV1A_t		uTypeHash;
	std::any	pValue;
};

/*
 * CONFIGURATION
 * cheat variables file manipulating (save/load/remove)
 */
namespace C // @credits: ducarii
{
	// Main
	/* create directories, create and load default config */
	bool Setup(std::string_view szDefaultFileName);
	/* write values in config file */
	bool Save(std::string_view szFileName);
	/* read values from config file */
	bool Load(std::string_view szFileName);

	bool ImportClipboard(std::string_view szFileName);
	bool ExportClipboard(std::string_view szFileName);

	/* remove config file */
	void Remove(std::string_view szFileName);
	/* loop through directory content and push config filenames to vector */
	void Refresh();

	// Get
	/* return variable index by hashed name */
	std::size_t GetVariableIndex(const FNV1A_t uNameHash);
	/* get path where output files will be saved (default: "%userprofile%\documents\.qo0") */
	std::filesystem::path GetWorkingPath();

	/* returns casted variable at given index */
	template <typename T>
	T& Get(const std::uint32_t nIndex)
	{
		return vecVariables.at(nIndex).Get<T>();
	}

	// Template
	template <typename T>
	std::uint32_t AddVariable(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, const T pDefault)
	{
		vecVariables.push_back(VariableObject_t(uNameHash, uTypeHash, std::make_any<T>(pDefault)));
		return vecVariables.size() - 1U;
	}

	// Values
	/* default configs path */
	const std::filesystem::path fsPath = GetWorkingPath();
	/* all user config filenames */
	inline std::deque<std::string> vecFileNames = { };
	/* configuration variables */
	inline std::vector<VariableObject_t> vecVariables = { };
}
