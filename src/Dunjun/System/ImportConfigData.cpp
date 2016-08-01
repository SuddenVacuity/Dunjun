
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

	ConfigData::Entry getConfigDataVariable(const ConfigData& data, const String& name)
	{
		return get(data.map, murmurStringHash64(name), { 0, ConfigType::ConfigType_Unknown });
	}

	ConfigData loadConfigDataFromFile(const String& filepath)
	{
		//String filepath = "data/defaultSettings.op";
		std::cout << "Importing Config File: " + filepath + "\n\n";

		std::fstream file;
		file.open(cString(filepath), std::ios::in | std::ios::binary);
		defer(file.close());

		if (!file.is_open())
			showSimpleMessageBox("config file failed to open: " + filepath);

		ConfigData configData = {};

		uSize_t lineNum = 0;
		String line;
		while (file.good())
		{
			defer(lineNum++);

			getline(file, line);
			line = Strings::trimSpace(line);

			uSize_t lineLen = len(line);

			// check for a comment at the beginning of the line
			if (line[0] == '/' && line[1] == '/')
				lineLen = 0;

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
						// set lineLen to ignore all comment text
						lineLen = i;
						break;
					}
				}

				// check for tokens
				if (line[i] == ':' && declarationPos == -1)
					declarationPos = i;
				else if (line[i] == '=' && definitionPos == -1)
					definitionPos = i;
			}

			// check if tokens were found
			if (declarationPos == -1 || definitionPos == -1)
			{
				showSimpleMessageBox("Parsing Error in config file:\n" +
					filepath + "\n\n" + line,
					"Parsing Error", MessageBoxType::Warning);
				continue;
			}

			// seperate each portion into seperate strings
			String type = substring(line, 0, declarationPos);
			String name = substring(line, declarationPos + 1, definitionPos);
			String value = substring(line, definitionPos + 1, lineLen);

			// remove whitespace
			type = Strings::toLowerCase(Strings::trimSpace(type));
			name = Strings::trimSpace(name);
			value = Strings::trimSpace(value);

			bool succeeded;

			if (type == "u_int")
				succeeded = addToConfigFile_uint(configData, name, value);
			else if (type == "s_int")
				succeeded = addToConfigFile_sint(configData, name, value);
			else if (type == "float")
				succeeded = addToConfigFile_float(configData, name, value);
			else if (type == "bool")
				succeeded = addToConfigFile_bool(configData, name, value);
			else if (type == "string")
				succeeded = addToConfigFile_string(configData, name, value);
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

			// get data through its hash
			ConfigData::Entry v = getConfigDataVariable(configData, name);
			switch (v.type)
			{
			default: std::cout << "Unlisted value - Should never get here\n"; break;
			case ConfigType::ConfigType_Unknown: std::cout << "Unknown Type - Should never get here\n"; break;
			case ConfigType::ConfigType_Uint:	 std::cout << type + "  : " + name + " " << configData.uints[v.index] << std::endl; break;
			case ConfigType::ConfigType_Sint:	 std::cout << type + "  : " + name + " " << configData.sints[v.index] << std::endl; break;
			case ConfigType::ConfigType_Float:	 std::cout << type + "  : " + name + " " << configData.floats[v.index] << std::endl; break;
			case ConfigType::ConfigType_Bool:	 std::cout << type + "   : " + name + " " << configData.bools[v.index] << std::endl; break;
			case ConfigType::ConfigType_String:	 std::cout << type + " : " + name + " \"" << configData.strings[v.index] << "\"" << std::endl; break;
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

		NSTOP
		std::cout << "\nFinish Importing Config File: " + filepath + "\n\n";

		return configData;
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					VALUE TYPE CONVERSIONS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	bool addToConfigFile_uint(ConfigData& data, const String& name, const String& value)
	{
		uSize_t i = 0;

		if (value[0] == '+')
			i = 1;
		else if (value[0] == '-')
		{
			std::cerr << "WARNING: " << name + " must be a positive number\n";
			return false; // add value failed
		}

		for (/**/; i < len(value); i++)
		{
			if (!Strings::isNumeric(value[i]))
			{
				std::cerr << "WARNING: " << name + " is not a number\n";
				return false; // add value failed
			}
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		u32 uint = atoi(cString(value));

		ConfigData::Entry e = {};
		e.type = ConfigType_Uint;
		e.index = 0;

		if (len(data.uints) > 0)
			e.index = len(data.uints);

		set(data.map, murmurStringHash64(name), e);
		append(data.uints, uint);

		return true;
	}

	bool addToConfigFile_sint(ConfigData& data, const String& name, const String& value)
	{
		uSize_t i = 0;
		if (value[0] == '+' || value[0] == '-')
			i = 1;

		for (/**/; i < len(value); i++)
		{
			if (!Strings::isNumeric(value[i]))
			{
				std::cerr << "WARNING: " << name + " is not a number\n";
				return false; // add value failed
			}
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		s32 sint = atoi(cString(value));

		ConfigData::Entry e = {};
		e.type = ConfigType_Sint;
		e.index = 0;

		if (len(data.sints) > 0)
			e.index = len(data.sints);

		set(data.map, murmurStringHash64(name), e);
		append(data.sints, sint);

		return true;
	}

	bool addToConfigFile_float(ConfigData& data, const String& name, const String& value)
	{
		f32 f;

		if (sscanf_s(cString(value), "%f", &f) != 1)
		{
			std::cerr << "WARNING: " << name + " is not a float\n";
			return false; // value is not a float
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		ConfigData::Entry e = {};
		e.type = ConfigType_Float;
		e.index = 0;

		if (len(data.floats) > 0)
			e.index = len(data.floats);

		set(data.map, murmurStringHash64(name), e);
		append(data.floats, f);

		return true;
	}

	bool addToConfigFile_bool(ConfigData& data, const String& name, const String& value)
	{
		b8 b;

		if (value == '1' || Strings::toLowerCase(value) == "true")
			b = true;
		else if (value == '0' || Strings::toLowerCase(value) == "false")
			b = false;
		else
		{
			std::cerr << "WARNING: " << name + " is not a boolean\n";
			return false; // add value failed
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			std::cerr << "WARNING: " << name << " already exists\n";
			return false; // add value failed
		}

		ConfigData::Entry e = {};
		e.type = ConfigType_Bool;
		e.index = 0;

		if (len(data.bools) > 0)
			e.index = len(data.bools);

		set(data.map, murmurStringHash64(name), e);
		append(data.bools, b);

		return true;
	}

	bool addToConfigFile_string(ConfigData& data, const String& name, const String& value)
	{
		// chech that data is long enough to contain quotation marks
		if (len(value) <= 2)
		{
			std::cerr << "WARNING: " << name + " value must have \" \" to be considered a string\n";
			return false;
		}

		// check for quotation marks 
		if (!(value[0] == '\"' && value[len(value) - 1] == '\"'))
		{
			std::cerr << "WARNING: " << name + " value must be contained within quotation marks\n";
			return false; // add value failed
		}

		if (has(data.map, murmurStringHash64(name)))
		{
			std::cerr << "WARNING: " << name << " already exists\n";
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


} // end Dunjun
