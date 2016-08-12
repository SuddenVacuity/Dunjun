
#include <Dunjun/System/ImportConfigData.hpp>

namespace Dunjun
{
	ConfigData::ConfigData()
		: uints(defaultAllocator())
		, sints(defaultAllocator())
		, floats(defaultAllocator())
		, bools(defaultAllocator())
		, stringsLength(0)
		, strings()
		, map(defaultAllocator())
	{
	}

	ConfigData::Entry getConfigDataEntry(const ConfigData& data, const String& name)
	{
		return get(data.map, murmurStringHash64(name), { 0, ConfigType::ConfigType_Unknown });
	}

	ConfigData loadConfigDataFromFile(const String& filepath)
	{
		//String filepath = "data/defaultSettings.op";
		//std::cout << "Start Importing Config File: " + filepath + "\n";

		std::fstream file;
		file.open(cString(filepath), std::ios::in | std::ios::binary);
		defer(file.close());

		if (!file.is_open())
			showSimpleMessageBox("config file failed to open: " + filepath);

		ConfigData configData = {};

		uSize_t lineNum = 0;
		String line;
		String section = {};

		while (file.good())
		{
			defer(lineNum++);

			getline(file, line);
			line = Strings::trimSpace(line);

			uSize_t lineLen = len(line);

			// check for a comment at the beginning of the line
			if (line[0] == '/' && line[1] == '/')
				continue;

			// if line is empty continue
			if (lineLen == 0)
				continue;

			sSize_t definitionPos = -1; // value
			sSize_t declarationPos = -1; // name

			// read the line
			for (sSize_t i = 0; i < lineLen; i++)
			{
				// check for comments
				if (line[i] == '/' && i != lineLen)
				{
					if (line[i + 1] == '/')
					{
						// set lineLen to where comment begins to ignore comment
						lineLen = i;
						break;
					}
				}

				// check for config section markers []
				if(line[0] == '[')
				{
					// remove any space that could be between ] and a removed comment
					line = Strings::trimSpace(line);
					lineLen = len(line);
					if(lineLen > 1 && line[lineLen - 1] == ']')
					{

						//if(lineLen == 2)
						//	section = "GLOBAL";
						//else
							section = Strings::trim(line, "[]");

						// set tokens as non-existing
						declarationPos = -2;
						definitionPos = -2;

						break;
					}
					else
					{
						logPrint(g_loggerInfo, "Import Config Section Error");
						//std::cout << "\nconfig section error\n\n";
						break;
					}
				} // end check for config section markers []

				// check that tokens were defined
				if (line[i] == ':' && declarationPos == -1)
					declarationPos = i;
				else if (line[i] == '=' && definitionPos == -1)
					definitionPos = i;
			} // end read line

			// check if tokens were found
			// -1 means they were never defined
			if(declarationPos == -1 || definitionPos == -1)
			{
				showSimpleMessageBox("Parsing Error in config file:\n" +
									 filepath + "\n\n" + line,
									 "Parsing Error", MessageBoxType::Warning);

				continue;
			}

			// check if tokens were found
			// both -2 means a section tag was found
			if (declarationPos == -2 && definitionPos == -2)
			{
				logPrint(g_loggerInfo, "%s", cString(line));
				//std::cout << "\n" + line + "\n";
				continue;
			}

			// seperate each portion into seperate strings
			String type = substring(line, 0, declarationPos);
			String name = substring(line, declarationPos + 1, definitionPos);
			String value = substring(line, definitionPos + 1, lineLen);

			// remove whitespace
			type = Strings::trimSpace(type);
			name = Strings::trimSpace(name);
			value = Strings::trimSpace(value);

			// add section prefix
			if (section != "")
				name = section + "." + name;

			bool succeeded;

			// organize by type
			if (type == "u_int")
				succeeded = addToConfigData_uint(configData, name, value);
			else if (type == "s_int")
				succeeded = addToConfigData_sint(configData, name, value);
			else if (type == "float")
				succeeded = addToConfigData_float(configData, name, value);
			else if (type == "bool")
				succeeded = addToConfigData_bool(configData, name, value);
			else if (type == "string")
				succeeded = addToConfigData_string(configData, name, value);
			else
			{
				succeeded = false;
				showSimpleMessageBox("Unknown data type in config file:\n" +
					filepath + "\n\n" + line,
					"Unknown Type", MessageBoxType::Warning);
				continue;
			}

			if (succeeded == false)
				continue;

			// get data through its hash and print to console
			ConfigData::Entry v = getConfigDataEntry(configData, name);
			switch (v.type)
			{
			default: logPrint(g_loggerInfo, "Unlisted value - Should never get here"); break;
			case ConfigType::ConfigType_Unknown: logPrint(g_loggerInfo, "Unknown Type - Should never get here"); break;
			case ConfigType::ConfigType_Uint:	 logPrint(g_loggerInfo, "%s  : %s = %d",	 cString(type), cString(name), configData.uints[v.index]); break;
			case ConfigType::ConfigType_Sint:	 logPrint(g_loggerInfo, "%s  : %s = %i",	 cString(type), cString(name), configData.sints[v.index]); break;
			case ConfigType::ConfigType_Float:	 logPrint(g_loggerInfo, "%s  : %s = %f",	 cString(type), cString(name), configData.floats[v.index]); break;
			case ConfigType::ConfigType_Bool:	 logPrint(g_loggerInfo, "%s   : %s = %d",	 cString(type), cString(name), configData.bools[v.index]); break;
			case ConfigType::ConfigType_String:	 logPrint(g_loggerInfo, "%s : %s = \"%s\"", cString(type), cString(name), cString( configData.strings[v.index])); break;
			}

		} // end while(file.good())

		//// confirm data was added
		//std::cout << "\n\nOutput all data\n";
		//const char* boolNames[] = { "false", "true" };
		//
		//std::cout << "\nuints:\n";
		//for (u32 u : configData.uints)
		//	std::cout << u << "\n";
		//
		//std::cout << "\nsints:\n";
		//for (s32 s : configData.sints)
		//	std::cout << s << "\n";
		//
		//std::cout << "\nfloats:\n";
		//for (f32 f : configData.floats)
		//	std::cout << f << "\n";
		//
		//std::cout << "\nbools:\n";
		//for (b8 b : configData.bools)
		//	std::cout << boolNames[b] << "\n";
		//
		//std::cout << "\nstrings:\n";
		//for (u32 i = 0; i < configData.stringsLength; i++)
		//	std::cout << cString(configData.strings[i]) << "\n";

		return configData;
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					ADD DATA VALUE TYPE CONVERSIONS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	bool addToConfigData_uint(ConfigData& data, const String& name, const String& value)
	{
		uSize_t i = 0;

		if (value[0] == '+')
			i = 1;
		else if (value[0] == '-')
		{
			logPrint(g_loggerInfo, "WARNING: %s value must be a positive number", cString(name));
			//std::cerr << "WARNING: " << name + " must be a positive number\n";
			return false; // add value failed
		}

		for (/**/; i < len(value); i++)
		{
			if (!Strings::isNumeric(value[i]))
			{
				logPrint(g_loggerInfo, "WARNING: %s value is not a number", cString(name));
				//std::cerr << "WARNING: " << name + " is not a number\n";
				return false; // add value failed
			}
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			logPrint(g_loggerInfo, "WARNING: %s already exists", cString(name));
			//std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		u32 uint = atoi(cString(value));

		ConfigData::Entry e = {};
		e.type = ConfigType_Uint;

		if (len(data.uints) > 0)
			e.index = len(data.uints);

		set(data.map, murmurStringHash64(name), e);
		append(data.uints, uint);

		return true;
	}

	bool addToConfigData_sint(ConfigData& data, const String& name, const String& value)
	{
		uSize_t i = 0;
		if (value[0] == '+' || value[0] == '-')
			i = 1;

		for (/**/; i < len(value); i++)
		{
			if (!Strings::isNumeric(value[i]))
			{
				logPrint(g_loggerInfo, "WARNING: %s value is not a number", cString(name));
				//std::cerr << "WARNING: " << name + " is not a number\n";
				return false; // add value failed
			}
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			logPrint(g_loggerInfo, "WARNING: %s already exists", cString(name));
			//std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		s32 sint = atoi(cString(value));

		ConfigData::Entry e = {};
		e.type = ConfigType_Sint;

		if (len(data.sints) > 0)
			e.index = len(data.sints);

		set(data.map, murmurStringHash64(name), e);
		append(data.sints, sint);

		return true;
	}

	bool addToConfigData_float(ConfigData& data, const String& name, const String& value)
	{
		f32 f;

		if (sscanf(cString(value), "%f", &f) != 1)
		{
			logPrint(g_loggerInfo, "WARNING: %s value is not a float", cString(name));
			//std::cerr << "WARNING: " << name + " is not a float\n";
			return false; // value is not a float
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			logPrint(g_loggerInfo, "WARNING: %s already exists", cString(name));
			//std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		ConfigData::Entry e = {};
		e.type = ConfigType_Float;

		if (len(data.floats) > 0)
			e.index = len(data.floats);

		set(data.map, murmurStringHash64(name), e);
		append(data.floats, f);

		return true;
	}

	bool addToConfigData_bool(ConfigData& data, const String& name, const String& value)
	{
		b8 b;

		if (value == '1' || Strings::toLowerCase(value) == "true")
			b = true;
		else if (value == '0' || Strings::toLowerCase(value) == "false")
			b = false;
		else
		{
			logPrint(g_loggerInfo, "WARNING: %s is not a boolean value", cString(name));
			//std::cerr << "WARNING: " << name + " is not a boolean\n";
			return false; // add value failed
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			logPrint(g_loggerInfo, "WARNING: %s already exists", cString(name));
			//std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		ConfigData::Entry e = {};
		e.type = ConfigType_Bool;

		if (len(data.bools) > 0)
			e.index = len(data.bools);

		set(data.map, murmurStringHash64(name), e);
		append(data.bools, b);

		return true;
	}

	bool addToConfigData_string(ConfigData& data, const String& name, const String& value)
	{
		// chech that data is long enough to contain quotation marks
		if (len(value) <= 2)
		{
			logPrint(g_loggerInfo, "WARNING: %s value must have \" \" to be considered a string", cString(name));
			//std::cerr << "WARNING: " << name + " value must have \" \" to be considered a string\n";
			return false;
		}

		// check for quotation marks 
		if (!(value[0] == '\"' && value[len(value) - 1] == '\"'))
		{
			logPrint(g_loggerInfo, "WARNING: %s value must be contained within quotation marks", cString(name));
			//std::cerr << "WARNING: " << name + " value must be contained within quotation marks\n";
			return false; // add value failed
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			logPrint(g_loggerInfo, "WARNING: %s already exists", cString(name));
			//std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		ConfigData::Entry e = {};
		e.type = ConfigType_String;
		e.index = data.stringsLength;

		set(data.map, murmurStringHash64(name), e);

		// start +1 and end -1 to remove quotation marks
		data.strings[e.index] = substring(value, 1, len(value) - 1);
		data.stringsLength++;

		return true;
	}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					GET DATA
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	u32 getFromConfigData_uint(const ConfigData& data, const String& name, const u32& defaultValue)
	{
		ConfigData::Entry e = getConfigDataEntry(data, name);

		if(e.type == ConfigType::ConfigType_Uint)
		{
			const u32 found = data.uints[e.index];
			logPrint(g_loggerInfo, "u_int  : %s = %d", cString(name), found);
			//std::cout << "u_int  : " + name + " = " << found << "\n";
			return found;
		}

		logPrint(g_loggerInfo, "DEFAULT: %s", cString(name));
		//std::cout << "DEFAULT: " + name + "\n";
		
		return defaultValue;
	}

	s32 getFromConfigData_sint(const ConfigData& data, const String& name, const s32& defaultValue)
	{
		ConfigData::Entry e = getConfigDataEntry(data, name);

		if (e.type == ConfigType::ConfigType_Sint)
		{
			const s32 found = data.sints[e.index];
			logPrint(g_loggerInfo, "s_int  : %s = %i", cString(name), found);
			//std::cout << "s_int  : " + name + " = " << found << "\n";
			return found;
		}

		logPrint(g_loggerInfo, "DEFAULT: %s", cString(name));
		//std::cout << "DEFAULT: " + name + "\n";

		return defaultValue;
	}

	f32 getFromConfigData_float(const ConfigData& data, const String& name, const f32& defaultValue)
	{
		ConfigData::Entry e = getConfigDataEntry(data, name);

		if (e.type == ConfigType::ConfigType_Float)
		{
			const f32 found = data.floats[e.index];
			logPrint(g_loggerInfo, "float  : %s = %f", cString(name), found);
			//std::cout << "float  : " + name + " = " << found << "\n";
			return found;
		}

		logPrint(g_loggerInfo, "DEFAULT: %s", cString(name));
		//std::cout << "DEFAULT: " + name + "\n";

		return defaultValue;
	}

	b8 getFromConfigData_bool(const ConfigData& data, const String& name, const b8& defaultValue)
	{
		ConfigData::Entry e = getConfigDataEntry(data, name);

		if (e.type == ConfigType::ConfigType_Bool)
		{
			const b8 found = data.bools[e.index];
			logPrint(g_loggerInfo, "bool   : %s = %d", cString(name), found);
			//std::cout << "bool   : " + name + " = " << found << "\n";
			return found;
		}

		logPrint(g_loggerInfo, "DEFAULT: %s", cString(name));
		//std::cout << "DEFAULT: " + name + "\n";

		return defaultValue;
	}

	String getFromConfigData_string(const ConfigData& data, const String& name, const String& defaultValue)
	{
		ConfigData::Entry e = getConfigDataEntry(data, name);

		if (e.type == ConfigType::ConfigType_String)
		{
			const String found = data.strings[e.index];
			logPrint(g_loggerInfo, "string : %s = \"%s\"", cString(name), cString(found)); // string name = "found"
			//std::cout << "string : " + name + " = \"" << found << "\"\n";
			return found;
		}

		logPrint(g_loggerInfo, "DEFAULT: %s", cString(name));
		//std::cout << "DEFAULT: " + name + "\n";
		
		return defaultValue;
	}





} // end Dunjun
