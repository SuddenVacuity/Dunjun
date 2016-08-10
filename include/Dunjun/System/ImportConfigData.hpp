#ifndef DUNJUN_SYSTEM_IMPORTCONFIGDATA_HPP
#define DUNJUN_SYSTEM_IMPORTCONFIGDATA_HPP

#include <Dunjun/System/Logger.hpp>


namespace Dunjun
{
	enum ConfigType
	{
		// to add more types: add type name enum here
		//					  add Array<type> type to ConfigData struct
		//					  create parse function that converts string to type and adds to configData array
		//					  							  and creates an entry in ConfigData HashMap<Entry> map
		//					  get data with HashMap::get()

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

	ConfigData::Entry getConfigDataEntry(const ConfigData& data, const String& name);
	ConfigData loadConfigDataFromFile(const String& filepath);

	bool addToConfigData_uint(ConfigData& data, const String& name, const String& value);
	bool addToConfigData_sint(ConfigData& data, const String& name, const String& value);
	bool addToConfigData_float(ConfigData& data, const String& name, const String& value);
	bool addToConfigData_bool(ConfigData& data, const String& name, const String& value);
	bool addToConfigData_string(ConfigData& data, const String& name, const String& value);

	u32 getFromConfigData_uint(const ConfigData& data, const String& name, const u32& defaultValue = 0xFFFF);
	s32 getFromConfigData_sint(const ConfigData& data, const String& name, const s32& defaultValue = 0xFFFF);
	f32 getFromConfigData_float(const ConfigData& data, const String& name, const f32& defaultValue = 0xFFFF);
	b8 getFromConfigData_bool(const ConfigData& data, const String& name, const b8& defaultValue = false);
	String getFromConfigData_string(const ConfigData& data, const String& name, const String& defaultValue = "valueDoesNotExist");



} // end Dunjun

#endif
