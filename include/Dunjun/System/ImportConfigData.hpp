#ifndef DUNJUN_SYSTEM_IMPORTCONFIGDATA_HPP
#define DUNJUN_SYSTEM_IMPORTCONFIGDATA_HPP

#include <Dunjun/Common.hpp>


namespace Dunjun
{
	enum ConfigType
	{
		ConfigType_Unknown,

		ConfigType_Uint,
		ConfigType_Sint,
		ConfigType_Float,
		ConfigType_Bool,
		ConfigType_String,
	};

	struct ConfigData
	{
		Array<u32> uints;
		Array<s32> sints;
		Array<f32> floats;
		Array<b8> bools;

		uSize_t stringsLength;
		String strings[1024];

		// this tracks the data type and position in the array
		struct Entry
		{
			u32 index;
			ConfigType type;
		};

		HashMap<Entry> map;

		ConfigData();
		~ConfigData() = default;
	};

	ConfigData::Entry getConfigDataVariable(const ConfigData& data, const String& name);
	ConfigData loadConfigDataFromFile(const String& filepath);

	bool addToConfigFile_uint(ConfigData& data, const String& name, const String& value);
	bool addToConfigFile_sint(ConfigData& data, const String& name, const String& value);
	bool addToConfigFile_float(ConfigData& data, const String& name, const String& value);
	bool addToConfigFile_bool(ConfigData& data, const String& name, const String& value);
	bool addToConfigFile_string(ConfigData& data, const String& name, const String& value);





} // end Dunjun

#endif
