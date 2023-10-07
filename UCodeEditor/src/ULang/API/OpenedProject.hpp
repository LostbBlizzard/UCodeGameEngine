#pragma once
#include "APITypeDefs.hpp"
#include "UCodeLang/UCodeAnalyzer/Preprocessors/CppHelper.hpp"

UCodeAPIEditorStart



UCodeLangExportSymbol("UCodeGameEngine") struct AssetDir
{
	AssetDir()
	{

	}
private:
	Path Base;
};


UCodeLangExportSymbol("UCodeGameEngine") struct AssetPath
{

private:
	Path Base;
};

UCodeLangExportSymbol("UCodeGameEngine") struct OpenedProject
{

	/// <summary>
	/// Gets the Full Opened Project Path on the disc.
	/// 
	/// Example: UCode/Projects/Snake/  
	/// </summary>
	/// <returns></returns>
	UCodeLangExport static Path GetProjectDirPath();


	/// <summary>
	/// Converts a Path into an AssetPath
	/// May Return nothing if its not the ProjectDir.   
	/// </summary>
	/// <returns></returns>
	UCodeLangExport inline static Optional<AssetPath> AsAssetPath(const Path& path)
	{
		AssetPath R;
		if (AsAssetPath(path,R))
		{
			return R;
		}
		return {};
	}

	/// <summary>
	/// Converts a Path into an AssetDir
	/// May Return false if its not the ProjectDir.  
	/// </summary>
	/// <returns></returns>
	UCodeLangExport inline static bool AsAssetPath(const Path& path, UCodeLangOutPartype(AssetPath) Outpath)
	{
		*Outpath = AssetPath();
		return  AsAssetPath(path, Outpath);
	}

	/// <summary>
	/// Converts a Path into an AssetDir
	/// May Return false if its not the ProjectDir.  
	/// </summary>
	/// <returns></returns>
	UCodeLangExport static bool AsAssetPath(const Path& path, AssetPath& Outpath);

	UCodeLangExport inline static Optional<AssetDir> AsAssetDir(const Path& path)
	{
		AssetDir R;
		if (AsAssetDir(path, R))
		{
			return R;
		}
		return {};
	}

	/// <summary>
	/// Converts a Path into an AssetDir
	/// May Return false if its not the ProjectDir.  
	/// </summary>
	/// <returns></returns>
	UCodeLangExport inline static bool AsAssetDir(const Path& path, UCodeLangOutPartype(AssetDir) Outpath)
	{
		*Outpath = AssetDir();
		return AsAssetDir(path, Outpath);
	}

	/// <summary>
	/// Converts a Path into an AssetDir
	/// May Return false if its not the ProjectDir.  
	/// </summary>
	/// <returns></returns>
	UCodeLangExport static bool AsAssetDir(const Path& path, AssetDir& Outpath);
};

UCodeAPIEditorEnd