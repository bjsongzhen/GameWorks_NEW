// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Linq;
using System.Xml.Serialization;
using System.ComponentModel;
using System.Runtime.Serialization;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
	/// <summary>
	/// Type of a build product
	/// </summary>
	public enum BuildProductType
	{
		/// <summary>
		/// An executable file
		/// </summary>
		Executable,

		/// <summary>
		/// A dynamically loaded module.
		/// </summary>
		DynamicLibrary,

		/// <summary>
		/// A statically linked library. Not required for the executable to run.
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// An import library. Not required for the executable to run.
		/// </summary>
		ImportLibrary,

		/// <summary>
		/// A symbol file. Not required for the executable to run.
		/// </summary>
		SymbolFile,

		/// <summary>
		/// A map file. Not required for the executable to run.
		/// </summary>
		MapFile,

		/// <summary>
		/// A resource file which was generated by the build and is required for the executable to run.
		/// </summary>
		RequiredResource,

		/// <summary>
		/// A build resource which was generated by the build, but is not required for the executable to run.
		/// </summary>
		BuildResource,

		/// <summary>
		/// A package which can be deployed on device (eg. *.apk for Android, *.stub for iOS)
		/// </summary>
		Package
	}

	/// <summary>
	/// A file that was created as part of the build process
	/// </summary>
	[Serializable]
	public class BuildProduct
	{
		/// <summary>
		/// Path to the file.
		/// </summary>
		public FileReference Path;

		/// <summary>
		/// Type of the build product.
		/// </summary>
		public BuildProductType Type;

		/// <summary>
		/// Whether the file is precompiled for use by downstream builds, but not directly used by the current target.
		/// </summary>
		public bool IsPrecompiled;

		/// <summary>
		/// Private constructor, for serialization.
		/// </summary>
		private BuildProduct()
		{
		}

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="InPath">Path to the build product</param>
		/// <param name="InType">Type of the build product</param>
		public BuildProduct(FileReference InPath, BuildProductType InType)
		{
			Path = InPath;
			Type = InType;
		}

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="Other">Build product to copy settings from</param>
		public BuildProduct(BuildProduct Other)
		{
			Path = Other.Path;
			Type = Other.Type;
			IsPrecompiled = Other.IsPrecompiled;
		}

		/// <summary>
		/// Convert this object to a string, for debugging.
		/// </summary>
		/// <returns>Path to this build product</returns>
		public override string ToString()
		{
			return Path.ToString();
		}
	}

	/// <summary>
	/// How a file may be staged
	/// </summary>
	public enum StagedFileType
	{
		/// <summary>
		/// Only accessed through Unreal filesystem functions; may be included in a PAK file.
		/// </summary>
		UFS,

		/// <summary>
		/// Must be kept as part of the loose filesystem.
		/// </summary>
		NonUFS,

		/// <summary>
		/// Debug file which must be kept as part of the loose filesystem.
		/// </summary>
		DebugNonUFS,

		/// <summary>
		/// System file which must be kept as part of the loose filesystem. System files are not subject to being automatic remapping or renaming by the platform layer.
		/// </summary>
		SystemNonUFS,
	}

	/// <summary>
	/// Information about a file which is required by the target at runtime, and must be moved around with it.
	/// </summary>
	[Serializable]
	public class RuntimeDependency
	{
		/// <summary>
		/// The file that should be staged. Should use $(EngineDir) and $(ProjectDir) variables as a root, so that the target can be relocated to different machines.
		/// </summary>
		public FileReference Path;

		/// <summary>
		/// How to stage this file.
		/// </summary>
		public StagedFileType Type;

		/// <summary>
		/// Private constructor, for serialization.
		/// </summary>
		private RuntimeDependency()
		{
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="InPath">Path to the runtime dependency</param>
		/// <param name="InType">How to stage the given path</param>
		public RuntimeDependency(FileReference InPath, StagedFileType InType = StagedFileType.NonUFS)
		{
			Path = InPath;
			Type = InType;
		}

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="InOther">Runtime dependency to copy settings from</param>
		public RuntimeDependency(RuntimeDependency InOther)
		{
			Path = InOther.Path;
			Type = InOther.Type;
		}

		/// <summary>
		/// Convert this object to a string for debugging
		/// </summary>
		/// <returns>String representation of the object</returns>
		public override string ToString()
		{
			return Path.ToString();
		}
	}

	/// <summary>
	/// List of runtime dependencies, with convenience methods for adding new items
	/// </summary>
	[Serializable]
	public class RuntimeDependencyList : List<RuntimeDependency>
	{
		/// <summary>
		/// Default constructor
		/// </summary>
		public RuntimeDependencyList()
		{
		}

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="Other">Sequence of runtime dependencies to initialize with</param>
		public RuntimeDependencyList(IEnumerable<RuntimeDependency> Other) : base(Other)
		{
		}

		/// <summary>
		/// Add a runtime dependency to the list
		/// </summary>
		/// <param name="InPath">Path to the runtime dependency. May include wildcards.</param>
		/// <param name="InType">How to stage this file</param>
		public void Add(FileReference InPath, StagedFileType InType)
		{
			Add(new RuntimeDependency(InPath, InType));
		}
	}

	/// <summary>
	/// Arbitrary property name/value which metadata from the build scripts can be passed on to downstream tasks
	/// </summary>
	[Serializable]
	public class ReceiptProperty
	{
		/// <summary>
		/// Property name
		/// </summary>
		[XmlAttribute]
		public string Name;

		/// <summary>
		/// Value of the property
		/// </summary>
		[XmlAttribute]
		public string Value;

		/// <summary>
		/// Construct a property with the given name and value
		/// </summary>
		/// <param name="InName">Name of the property</param>
		/// <param name="InValue">Value of the property</param>
		public ReceiptProperty(string InName, string InValue)
		{
			Name = InName;
			Value = InValue;
		}
	}

	/// <summary>
	/// Stores a record of a built target, with all metadata that other tools may need to know about the build.
	/// </summary>
	[Serializable]
	public class TargetReceipt
	{
		/// <summary>
		/// The name of this target
		/// </summary>
		public string TargetName;

		/// <summary>
		/// Which platform the target is compiled for
		/// </summary>
		public UnrealTargetPlatform Platform;

		/// <summary>
		/// Which configuration this target is compiled in
		/// </summary>
		public UnrealTargetConfiguration Configuration;

		/// <summary>
		/// Version information for this target.
		/// </summary>
		public BuildVersion Version;

		/// <summary>
		/// The build products which are part of this target
		/// </summary>
		public List<BuildProduct> BuildProducts = new List<BuildProduct>();

		/// <summary>
		/// All the runtime dependencies that this target relies on
		/// </summary>
		public RuntimeDependencyList RuntimeDependencies = new RuntimeDependencyList();

		/// <summary>
		/// All the files which are required to use precompiled binaries with this target
		/// </summary>
		public HashSet<FileReference> PrecompiledBuildDependencies = new HashSet<FileReference>();

		/// <summary>
		/// All the files which are required runtime dependencies for precompiled binaries that are part of this target
		/// </summary>
		public HashSet<FileReference> PrecompiledRuntimeDependencies = new HashSet<FileReference>();

		/// <summary>
		/// Additional build properties passed through from the module rules
		/// </summary>
		public List<ReceiptProperty> AdditionalProperties = new List<ReceiptProperty>();

		/// <summary>
		/// Default constructor
		/// </summary>
		public TargetReceipt()
		{
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="InTargetName">The name of the target being compiled</param>
		/// <param name="InPlatform">Platform for the target being compiled</param>
		/// <param name="InConfiguration">Configuration of the target being compiled</param>
		/// <param name="InVersion">Version information for the target</param>
		public TargetReceipt(string InTargetName, UnrealTargetPlatform InPlatform, UnrealTargetConfiguration InConfiguration, BuildVersion InVersion)
		{
			TargetName = InTargetName;
			Platform = InPlatform;
			Configuration = InConfiguration;
			Version = InVersion;
		}

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="Other">Receipt to copy from</param>
		public TargetReceipt(TargetReceipt Other)
		{
			foreach (BuildProduct OtherBuildProduct in Other.BuildProducts)
			{
				BuildProducts.Add(new BuildProduct(OtherBuildProduct));
			}
			foreach (RuntimeDependency OtherRuntimeDependency in Other.RuntimeDependencies)
			{
				RuntimeDependencies.Add(new RuntimeDependency(OtherRuntimeDependency));
			}
			AdditionalProperties.AddRange(Other.AdditionalProperties);
			PrecompiledBuildDependencies.UnionWith(Other.PrecompiledBuildDependencies);
			PrecompiledRuntimeDependencies.UnionWith(Other.PrecompiledRuntimeDependencies);
		}

		/// <summary>
		/// Adds a build product to the receipt. Does not check whether it already exists.
		/// </summary>
		/// <param name="Path">Path to the build product.</param>
		/// <param name="Type">Type of build product.</param>
		/// <returns>The BuildProduct object that was created</returns>
		public BuildProduct AddBuildProduct(FileReference Path, BuildProductType Type)
		{
			BuildProduct NewBuildProduct = new BuildProduct(Path, Type);
			BuildProducts.Add(NewBuildProduct);
			return NewBuildProduct;
		}

		/// <summary>
		/// Merges another receipt to this one.
		/// </summary>
		/// <param name="Other">Receipt which should be merged</param>
		public void Merge(TargetReceipt Other)
		{
			foreach (BuildProduct OtherBuildProduct in Other.BuildProducts)
			{
				BuildProducts.Add(OtherBuildProduct);
			}
			foreach (RuntimeDependency OtherRuntimeDependency in Other.RuntimeDependencies)
			{
				if (!RuntimeDependencies.Any(x => x.Path == OtherRuntimeDependency.Path))
				{
					RuntimeDependencies.Add(OtherRuntimeDependency);
				}
			}
			PrecompiledBuildDependencies.UnionWith(Other.PrecompiledBuildDependencies);
			PrecompiledRuntimeDependencies.UnionWith(Other.PrecompiledRuntimeDependencies);
		}

		/// <summary>
		/// Inserts variables to make a file relative to $(EngineDir) or $(ProjectDir)
		/// </summary>
		/// <param name="File">The file to insert variables into.</param>
		/// <param name="EngineDir">Value of the $(EngineDir) variable.</param>
		/// <param name="ProjectDir">Value of the $(ProjectDir) variable.</param>
		/// <returns>Converted path for the file.</returns>
		public static string InsertPathVariables(FileReference File, DirectoryReference EngineDir, DirectoryReference ProjectDir)
		{
			if (File.IsUnderDirectory(EngineDir))
			{
				return "$(EngineDir)/" + File.MakeRelativeTo(EngineDir).Replace(Path.DirectorySeparatorChar, '/');
			}
			else if (ProjectDir != null && File.IsUnderDirectory(ProjectDir))
			{
				return "$(ProjectDir)/" + File.MakeRelativeTo(ProjectDir).Replace(Path.DirectorySeparatorChar, '/');
			}
			else
			{
				return File.FullName;
			}
		}

		/// <summary>
		/// Inserts variables to make a file relative to $(EngineDir) or $(ProjectDir)
		/// </summary>
		/// <param name="Path">The path to insert variables into.</param>
		/// <param name="EngineDir">Value of the $(EngineDir) variable.</param>
		/// <param name="ProjectDir">Value of the $(ProjectDir) variable.</param>
		/// <returns>Converted path for the file.</returns>
		public static FileReference ExpandPathVariables(string Path, DirectoryReference EngineDir, DirectoryReference ProjectDir)
		{
			const string EnginePrefix = "$(EngineDir)";
			if(Path.StartsWith(EnginePrefix, StringComparison.InvariantCultureIgnoreCase))
			{
				return new FileReference(EngineDir.FullName + Path.Substring(EnginePrefix.Length));
			}

			const string ProjectPrefix = "$(ProjectDir)";
			if(ProjectDir != null && Path.StartsWith(ProjectPrefix, StringComparison.InvariantCultureIgnoreCase))
			{
				return new FileReference(ProjectDir.FullName + Path.Substring(ProjectPrefix.Length));
			}

			return new FileReference(Path);
		}

		/// <summary>
		/// Returns the standard path to the build receipt for a given target
		/// </summary>
		/// <param name="BaseDir">Base directory for the target being built; either the project directory or engine directory.</param>
		/// <param name="TargetName">The target being built</param>
		/// <param name="Platform">The target platform</param>
		/// <param name="Configuration">The target configuration</param>
		/// <param name="BuildArchitecture">The architecture being built</param>
		/// <returns>Path to the receipt for this target</returns>
		public static FileReference GetDefaultPath(DirectoryReference BaseDir, string TargetName, UnrealTargetPlatform Platform, UnrealTargetConfiguration Configuration, string BuildArchitecture)
		{
			// Get the architecture suffix. Platforms have the option of overriding whether to include this string in filenames.
			string ArchitectureSuffix = "";
			if(UEBuildPlatform.GetBuildPlatform(Platform).RequiresArchitectureSuffix())
			{
				ArchitectureSuffix = BuildArchitecture;
			}
		
			// Build the output filename
			if (String.IsNullOrEmpty(ArchitectureSuffix) && Configuration == UnrealTargetConfiguration.Development)
			{
				return FileReference.Combine(BaseDir, "Binaries", Platform.ToString(), String.Format("{0}.target", TargetName));
			}
			else
			{
				return FileReference.Combine(BaseDir, "Binaries", Platform.ToString(), String.Format("{0}-{1}-{2}{3}.target", TargetName, Platform.ToString(), Configuration.ToString(), ArchitectureSuffix));
			}
		}

		/// <summary>
		/// Read a receipt from disk.
		/// </summary>
		/// <param name="Location">Filename to read from</param>
		/// <param name="EngineDir">Engine directory for expanded variables</param>
		/// <param name="ProjectDir">Project directory for expanded variables</param>
		public static TargetReceipt Read(FileReference Location, DirectoryReference EngineDir, DirectoryReference ProjectDir)
		{
			JsonObject RawObject = JsonObject.Read(Location);

			// Read the initial fields
			string TargetName = RawObject.GetStringField("TargetName");
			UnrealTargetPlatform Platform = RawObject.GetEnumField<UnrealTargetPlatform>("Platform");
			UnrealTargetConfiguration Configuration = RawObject.GetEnumField<UnrealTargetConfiguration>("Configuration");

			// Try to read the build version
			BuildVersion Version;
			if (!BuildVersion.TryParse(RawObject.GetObjectField("Version"), out Version))
			{
				throw new JsonParseException("Invalid 'Version' field");
			}

			// Create the receipt
			TargetReceipt Receipt = new TargetReceipt(TargetName, Platform, Configuration, Version);

			// Read the build products
			JsonObject[] BuildProductObjects;
			if (RawObject.TryGetObjectArrayField("BuildProducts", out BuildProductObjects))
			{
				foreach (JsonObject BuildProductObject in BuildProductObjects)
				{
					string Path;
					BuildProductType Type;
					if (BuildProductObject.TryGetStringField("Path", out Path) && BuildProductObject.TryGetEnumField("Type", out Type))
					{
						FileReference File = ExpandPathVariables(Path, EngineDir, ProjectDir);

						string Module;
						BuildProductObject.TryGetStringField("Module", out Module);

						BuildProduct NewBuildProduct = Receipt.AddBuildProduct(File, Type);

						bool IsPrecompiled;
						if (BuildProductObject.TryGetBoolField("IsPrecompiled", out IsPrecompiled))
						{
							NewBuildProduct.IsPrecompiled = IsPrecompiled;
						}
					}
				}
			}

			// Read the runtime dependencies
			JsonObject[] RuntimeDependencyObjects;
			if (RawObject.TryGetObjectArrayField("RuntimeDependencies", out RuntimeDependencyObjects))
			{
				foreach (JsonObject RuntimeDependencyObject in RuntimeDependencyObjects)
				{
					string Path;
					if (RuntimeDependencyObject.TryGetStringField("Path", out Path))
					{
						FileReference File = ExpandPathVariables(Path, EngineDir, ProjectDir);

						StagedFileType Type;
						if(!RuntimeDependencyObject.TryGetEnumField("Type", out Type))
						{
							// Previous format included an optional IgnoreIfMissing flag, which was only used for debug files. We can explicitly reference them as DebugNonUFS files now.
							bool bIgnoreIfMissing;
							if(RuntimeDependencyObject.TryGetBoolField("IgnoreIfMissing", out bIgnoreIfMissing))
							{
								bIgnoreIfMissing = false;
							}
							Type = bIgnoreIfMissing? StagedFileType.DebugNonUFS : StagedFileType.NonUFS;
						}

						Receipt.RuntimeDependencies.Add(File, Type);
					}
				}
			}

			// Read the additional properties
			JsonObject[] AdditionalPropertyObjects;
			if(RawObject.TryGetObjectArrayField("AdditionalProperties", out AdditionalPropertyObjects))
			{
				foreach(JsonObject AdditionalPropertyObject in AdditionalPropertyObjects)
				{
					string Name;
					if(AdditionalPropertyObject.TryGetStringField("Name", out Name))
					{
						string Value;
						if(AdditionalPropertyObject.TryGetStringField("Value", out Value))
						{
							Receipt.AdditionalProperties.Add(new ReceiptProperty(Name, Value));
						}
					}
				}
			}

			// Read the precompiled dependencies
			string[] PrecompiledBuildDependencies;
			if(RawObject.TryGetStringArrayField("PrecompiledBuildDependencies", out PrecompiledBuildDependencies))
			{
				foreach(string PrecompiledBuildDependency in PrecompiledBuildDependencies)
				{
					FileReference File = ExpandPathVariables(PrecompiledBuildDependency, EngineDir, ProjectDir);
					Receipt.PrecompiledBuildDependencies.Add(File);
				}
			}

			// Read the precompiled dependencies
			string[] PrecompiledRuntimeDependencies;
			if(RawObject.TryGetStringArrayField("PrecompiledRuntimeDependencies", out PrecompiledRuntimeDependencies))
			{
				foreach(string PrecompiledRuntimeDependency in PrecompiledRuntimeDependencies)
				{
					FileReference File = ExpandPathVariables(PrecompiledRuntimeDependency, EngineDir, ProjectDir);
					Receipt.PrecompiledRuntimeDependencies.Add(File);
				}
			}

			return Receipt;
		}

		/// <summary>
		/// Try to read a receipt from disk, failing gracefully if it can't be read.
		/// </summary>
		/// <param name="Location">Filename to read from</param>
		/// <param name="EngineDir">Engine directory for expanded paths</param>
		/// <param name="ProjectDir">Project directory for expanded paths</param>
		/// <param name="Receipt">If successful, the receipt that was read</param>
		/// <returns>True if successful</returns>
		public static bool TryRead(FileReference Location, DirectoryReference EngineDir, DirectoryReference ProjectDir, out TargetReceipt Receipt)
		{
			if (!FileReference.Exists(Location))
			{
				Receipt = null;
				return false;
			}

			try
			{
				Receipt = Read(Location, EngineDir, ProjectDir);
				return true;
			}
			catch (Exception)
			{
				Receipt = null;
				return false;
			}
		}

		/// <summary>
		/// Write the receipt to disk.
		/// </summary>
		/// <param name="Location">Output filename</param>
		/// <param name="EngineDir">Engine directory for expanded paths</param>
		/// <param name="ProjectDir">Project directory for expanded paths</param>
		public void Write(FileReference Location, DirectoryReference EngineDir, DirectoryReference ProjectDir)
		{
			using (JsonWriter Writer = new JsonWriter(Location.FullName))
			{
				Writer.WriteObjectStart();
				Writer.WriteValue("TargetName", TargetName);
				Writer.WriteValue("Platform", Platform.ToString());
				Writer.WriteValue("Configuration", Configuration.ToString());

				Writer.WriteObjectStart("Version");
				Version.WriteProperties(Writer);
				Writer.WriteObjectEnd();

				Writer.WriteArrayStart("BuildProducts");
				foreach (BuildProduct BuildProduct in BuildProducts)
				{
					Writer.WriteObjectStart();
					Writer.WriteValue("Path", InsertPathVariables(BuildProduct.Path, EngineDir, ProjectDir));
					Writer.WriteValue("Type", BuildProduct.Type.ToString());
					if (BuildProduct.IsPrecompiled)
					{
						Writer.WriteValue("IsPrecompiled", BuildProduct.IsPrecompiled);
					}
					Writer.WriteObjectEnd();
				}
				Writer.WriteArrayEnd();

				Writer.WriteArrayStart("RuntimeDependencies");
				foreach (RuntimeDependency RuntimeDependency in RuntimeDependencies)
				{
					Writer.WriteObjectStart();
					Writer.WriteValue("Path", InsertPathVariables(RuntimeDependency.Path, EngineDir, ProjectDir));
					Writer.WriteValue("Type", RuntimeDependency.Type.ToString());
					Writer.WriteObjectEnd();
				}
				Writer.WriteArrayEnd();

				if(AdditionalProperties.Count > 0)
				{
					Writer.WriteArrayStart("AdditionalProperties");
					foreach (ReceiptProperty AdditionalProperty in AdditionalProperties)
					{
						Writer.WriteObjectStart();
						Writer.WriteValue("Name", AdditionalProperty.Name);
						Writer.WriteValue("Value", AdditionalProperty.Value);
						Writer.WriteObjectEnd();
					}
					Writer.WriteArrayEnd();
				}

				if(PrecompiledBuildDependencies.Count > 0)
				{
					Writer.WriteArrayStart("PrecompiledBuildDependencies");
					foreach(string PrecompiledBuildDependency in PrecompiledBuildDependencies.Select(x => InsertPathVariables(x, EngineDir, ProjectDir)).OrderBy(x => x))
					{
						Writer.WriteValue(PrecompiledBuildDependency);
					}
					Writer.WriteArrayEnd();
				}

				if(PrecompiledRuntimeDependencies.Count > 0)
				{
					Writer.WriteArrayStart("PrecompiledRuntimeDependencies");
					foreach(string PrecompiledRuntimeDependency in PrecompiledRuntimeDependencies.Select(x => InsertPathVariables(x, EngineDir, ProjectDir)).OrderBy(x => x))
					{
						Writer.WriteValue(PrecompiledRuntimeDependency);
					}
					Writer.WriteArrayEnd();
				}

				Writer.WriteObjectEnd();
			}
		}
	}
}
