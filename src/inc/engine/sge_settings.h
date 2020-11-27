#pragma once

#include "..\sge_string.h"
#include "..\sge_utils.h"
#include "..\utils\simpleini.h"



#define INI_PROPERTY_READER(CLSNAME) private: CSimpleIniA __reader; \
									 private: SGE::String __fileName;	\
									 public: CLSNAME(SGE::String fileName): __fileName(fileName) { __reader.LoadFile(fileName.AsStdString().c_str()); __reader.SetUnicode(); }
//
#define INI_PROPERTY_STRING(NAME,SECTION,KEY) SGE::String NAME() const { return SGE::String(__reader.GetValue(SECTION, KEY, ""));} \
										void NAME(SGE::String value) { __reader.SetValue(SECTION, KEY, value.AsStdString().c_str());}
//
#define INI_PROPERTY_STRING(NAME,SECTION) SGE::String NAME() const { return SGE::String(__reader.GetValue(SECTION, #NAME, ""));} \
										 void NAME(SGE::String value) { __reader.SetValue(SECTION, #NAME, value.AsStdString().c_str());}



// // // // // 
#define INI_PROPERTY_INT(NAME,SECTION,KEY) int NAME() const { return std::stoi(__reader.GetValue(SECTION, KEY, ""));} \
										   void NAME(int value) { __reader.SetValue(SECTION, KEY, SGE::String(value).AsStdString().c_str());}
//
#define INI_PROPERTY_INT(NAME,SECTION) int NAME() const { return std::stoi(__reader.GetValue(SECTION, #NAME, ""));} \
										 void NAME(int value) { __reader.SetValue(SECTION, #NAME, SGE::String(value).AsStdString().c_str());}

namespace SGE::Settings
{

	class SgeEngineSettings
	{
	public:
		INI_PROPERTY_READER(SgeEngineSettings)
			//
		INI_PROPERTY_INT(RefreshRate, "Video")
		INI_PROPERTY_INT(ResolutionHeight, "Video")
		INI_PROPERTY_INT(ResolutionWidth, "Video")
		//
		INI_PROPERTY_INT(EngineVersion, "General")
		INI_PROPERTY_INT(OSType, "General")
		INI_PROPERTY_INT(OSVersion, "General")

		void Save()
		{
			auto result = __reader.SaveFile(__fileName.AsStdString().c_str(), true);
			if (result == SI_FAIL) throw Exceptions::SGEIoException(__fileName, "Generic Error");
			if (result == SI_FILE) throw Exceptions::SGEIoException(__fileName, "Error saving to file");
				//result == SI_NOMEM
		}

			//
	};
}
