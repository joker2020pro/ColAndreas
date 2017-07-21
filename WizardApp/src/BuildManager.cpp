/*
* This file is part of ColAndreas.
* Copyright(C) <2013>  <christopher michael james>
* computer_customs@hotmail.com
*
* ColAndreas is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ColAndreas is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Special thanks:
*
* [uL]Slice, Writing the original collision reader function
*/

#include <sstream>
#include <QFile>

#include <include/QtMainWindow.hpp>

#include <include/BuildManager.h>
#include <include/ImgManager.hpp>
#include <include/ColManager.hpp>

// BuildManager constructor
BuildManager::BuildManager(std::string &GTADirectory, std::string &OutDirectory, bool SAMPObjects, bool CustomObjects, std::string &MpIdeFilePath, std::string &MpIplFilePath, std::string &MpImgFilePath)
    : GTAInstallDirectory(GTADirectory), OutfileDirectory(OutDirectory), UsingSAMPObjects(SAMPObjects), UsingCustomObjects(CustomObjects), MpIdePath(MpIdeFilePath), MpIplPath(MpIplFilePath), MpImgPath(MpImgFilePath)
{
	InitDirectoryNames();
}

void BuildManager::InitDirectoryNames()
{
	//----------[ IMG ]----------//
	
	ImageFileNames.push_back("\\models\\gta_int.img");
	ImageFileNames.push_back("\\models\\gta3.img");
    ImageFileNames.push_back("\\models\\gamemod.img");

    if(UsingSAMPObjects)
    {
        ImageFileNames.push_back("\\multiplayer_c\\models\\mp_col.img");
    }

    if(UsingCustomObjects)
    {
        ImageFileNames.push_back(MpImgPath);
    }

	//----------[ COL ]----------//
	
	CollisionFileNames.push_back("\\models\\coll\\weapons.col");
	CollisionFileNames.push_back("\\models\\coll\\vehicles.col");
	//CollisionFileNames.push_back("\\models\\coll\\peds.col"; //Unusual file structure causing problems, file is unused in game.

	//----------[ IPL ]----------//

    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki_add2.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki_add3.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\sveg.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\add_sopki.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki_lesa.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_N.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_N2.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_Ngen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\country_WN.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_veg.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_veg2.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_park.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_S.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\crteam\\crteam.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\crteam\\crteam_gen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\crteam\\crteam_veg.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\ryazan\\ryazan.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\ryazan\\ryazan_gen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\ryazan\\ryazan_veg.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\ryazan\\koryakino.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\ryazan\\koryakino_gen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\garel.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\garel_gen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\materik.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\materik_objs.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\materik_gen.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\SPC_generic.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\lytkarino\\lytkarino.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\lytkarino\\lyt_genveg.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\rogovichi.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\tesla_place.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\bigRyazanLes.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\OpenAir.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\batyrevo\\batyrevo.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\batyrevo\\batyrevo_tree.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\russia\\metro2.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\mod_int\\mod_int.IPL");
    ItemPlacementFileNames.push_back("\\DATA\\MAPS\\mod_int\\AudiosurfGame.IPL");

    if(UsingCustomObjects)
    {
        ItemPlacementFileNames.push_back(MpIplPath);
    }

	//----------[ IDE ]----------//
	
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\vegepart.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\barriers.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\dynamic.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\dynamic2.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\multiobj.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\generic\\procobj.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\leveldes\\levelxre.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\sopki\\newgen.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\sopki\\sopki_lesa.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_N.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_gen.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_park.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\Arzamas\\country_WN.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\Arzamas\\arzamas_S.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\crteam\\crteam.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\ryazan\\ryazan.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\russia\\garel.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\russia\\materik.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\russia\\materik_objs.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\lytkarino\\lytkarino.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\russia\\rogovichi.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\russia\\OpenAir.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\batyrevo\\batyrevo.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\mod_int\\mod_int.IDE");
    ItemDefinitionFileNames.push_back("\\DATA\\MAPS\\mod_int\\AudiosurfGame.IDE");
    ItemDefinitionFileNames.push_back("\\data\\maps\\veh_mods\\veh_mods.IDE");
	
    if(UsingSAMPObjects)
    {
        ItemDefinitionFileNames.push_back("\\multiplayer_c\\models\\mp.ide");
    }

    if(UsingCustomObjects)
    {
        ItemDefinitionFileNames.push_back(MpIdePath);
    }

	#ifndef WIN32
	for(std::string &s : ImageFileNames)
		std::replace(s.begin(), s.end(), '\\', '/');
	
	for(std::string &s : CollisionFileNames)
		std::replace(s.begin(), s.end(), '\\', '/');
	
	for(std::string &s : ItemPlacementFileNames)
		std::replace(s.begin(), s.end(), '\\', '/');
	
	for(std::string &s : ItemDefinitionFileNames)
		std::replace(s.begin(), s.end(), '\\', '/');
	#endif
}

bool BuildManager::ReadColFile(const char fname[])
{
	std::ifstream file(fname, std::ios::binary);

	if (!file.is_open())
	{//Will error on 2 files because of the path's
		//cout << "(Failed to open) Error on file: " << fname << '\n';
		return false;
	}

	ColFile fileReader(&file);
	ColModel modelData; 
	
	while(fileReader.Load(&modelData))
	{
		std::transform(modelData.name.begin(), modelData.name.end(), modelData.name.begin(), ::tolower);
		if(modelData.name.find("lod") == std::string::npos) COLArray.emplace_back(modelData);
	}

	return true;
}

bool BuildManager::ExtractImageFiles()
{
	for(std::string &name : ImageFileNames)
	{
        mainWindow->addFormattedMessage("\tОткрытие архива: %s", name.c_str());

        std::string path(GTAInstallDirectory + name);

        if(!QFile::exists(QString::fromStdString(path)))
        {
            path.assign(name);
        }

        ImgFile thisFile(path);
		
        if(thisFile.Load())
		{
			for(ImgEntry &e : thisFile.COL)
			{
                mainWindow->addFormattedMessage("\t\tЗагрузка: %s", e.name);
				
				thisFile.handle->seekg(e.offset * 2048);
				
				ColFile fileReader(thisFile.handle);
				ColModel modelInfo;
				
				while(fileReader.Load(&modelInfo))
				{
					std::transform(modelInfo.name.begin(), modelInfo.name.end(), modelInfo.name.begin(), ::tolower);
					if(modelInfo.name.find("lod") == std::string::npos) COLArray.emplace_back(modelInfo);
				}
			}
			
			for(ImgEntry &e : thisFile.IPL)
			{
                mainWindow->addFormattedMessage("\t\tЗагрузка: %s", e.name);
				
				char *data = new char[e.streamingSize * 2048];
				
				thisFile.handle->seekg(e.offset * 2048);
				thisFile.handle->read(data, e.streamingSize * 2048);
				
				ReadBinaryItemPlacementData(data);
				
				delete [] data;
			}
			
            mainWindow->addMessage("\t\tГотово!\n");
		}
        else
        {
            mainWindow->addMessage("\t\tОшибка: не удалось открыть файл!\n");
        }
	}
	
	return true;
}

//throws: invalid allocation size: 4294967295 error. I assume this is because these files are of version "COLL" / version 1
bool BuildManager::ExtractCollisionFiles()
{
	bool ReturnValue = true;
	for(std::string &col : CollisionFileNames)
	{
        mainWindow->addFormattedMessage("\tЗагрузка: %s", col.c_str());
		
		if (ReadColFile((GTAInstallDirectory + col).c_str()))
		{
            mainWindow->addMessage("\t\tГотово!");
		}
		else
		{
            mainWindow->addMessage("\t\tОшибка: не удалось открыть файл!");
			ReturnValue = false;
		}
	}
	return true;
}

void BuildManager::ReadBinaryItemPlacementData(char* RawIPLData)
{
	uint32_t NumberofItemInstances, FileOffset;

	memcpy(&NumberofItemInstances, RawIPLData + 4, 4);
	memcpy(&FileOffset, RawIPLData + 28, 4);

	uint32_t TempModelid;

	for (uint32_t i = 0; i < NumberofItemInstances; i++)
	{
		IPLObject thisObject;

		GetBytes(RawIPLData, thisObject.Position.x, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Position.y, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Position.z, FileOffset, 4);

		GetBytes(RawIPLData, thisObject.Rotation.x, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.y, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.z, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.w, FileOffset, 4);

		GetBytes(RawIPLData, TempModelid, FileOffset, 4);
		thisObject.modelid = static_cast<uint16_t>(TempModelid);

		std::transform(thisObject.Modelname.begin(), thisObject.Modelname.end(), thisObject.Modelname.begin(), ::tolower);
		IPLArray.emplace_back(thisObject);
		
		FileOffset += 8; //Skip Interiorid, lodflag
	}
}

bool BuildManager::ReadItemPlacementData(const char fname[])
{
	std::ifstream IPLFile(fname, std::ios::binary);

	if (IPLFile.is_open())
	{
		char IPLHeader[5];
		IPLHeader[4] = 0x00;
		
		IPLFile.read(IPLHeader, 4);
		IPLFile.seekg(0);
		
		if(strcmp(IPLHeader, "bnry") != 0)
		{
			std::string thisLine;
			bool inInstArea = false;
			
			while(std::getline(IPLFile, thisLine))
			{
				if(thisLine.at(0) == '#') continue;
				thisLine.erase(std::find_if(thisLine.rbegin(), thisLine.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisLine.end());
				
				if(thisLine == "inst")
				{
					inInstArea = true;
				}
				else if(thisLine == "end")
				{
					inInstArea = false;
				}
				else if(inInstArea)
				{
					std::stringstream paramStream(thisLine);
					std::string thisParam;
					unsigned int index = 0;
					IPLObject thisObject;
					
					while(std::getline(paramStream, thisParam, ','))
					{
						thisParam.erase(thisParam.begin(), std::find_if(thisParam.begin(), thisParam.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
						thisParam.erase(std::find_if(thisParam.rbegin(), thisParam.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisParam.end());
					
						index++;
						
						try
						{
							switch(index)
							{
								case 1:
								{
									thisObject.modelid = static_cast<uint16_t>(std::stoi(thisParam));
									break;
								}
								case 2:
								{
									thisObject.Modelname = thisParam;
									break;
								}
								case 4:
								{
									thisObject.Position.x = std::stof(thisParam);
									break;
								}
								case 5:
								{
									thisObject.Position.y = std::stof(thisParam);
									break;
								}
								case 6:
								{
									thisObject.Position.z = std::stof(thisParam);
									break;
								}
								case 7:
								{
									thisObject.Rotation.x = std::stof(thisParam);
									break;
								}
								case 8:
								{
									thisObject.Rotation.y = std::stof(thisParam);
									break;
								}
								case 9:
								{
									thisObject.Rotation.z = std::stof(thisParam);
									break;
								}
								case 10:
								{
									thisObject.Rotation.w = std::stof(thisParam);
									break;
								}
							}
						}
						catch(std::invalid_argument e)
						{
							return false;
						}
						catch(std::out_of_range e)
						{
							return false;
						}
					}
					
					std::transform(thisObject.Modelname.begin(), thisObject.Modelname.end(), thisObject.Modelname.begin(), ::tolower);
					IPLArray.emplace_back(thisObject);
				}
			}
			
			return true;
		}
	}
	
	return false;
}

bool BuildManager::ExtractItemPlacementFiles()
{
	bool ReturnValue = true;
	
	for(std::string &ipl : ItemPlacementFileNames)
	{
        std::string path(GTAInstallDirectory + ipl);

        if(!QFile::exists(QString::fromStdString(path)))
        {
            path.assign(ipl);
        }

        if (ReadItemPlacementData(path.c_str()))
		{
            mainWindow->addFormattedMessage("\tЗагружено: %s", ipl.c_str());
		}
		else
		{
            mainWindow->addFormattedMessage("\tНе удалось загрузить: %s", ipl.c_str());
			ReturnValue = false;
		}
	}
	
	return true;
}

bool BuildManager::ExtractItemDefinitionFiles()
{
	bool ReturnValue = true;
	
	for(std::string &ide : ItemDefinitionFileNames)
	{
        std::string path(GTAInstallDirectory + ide);

        if(!QFile::exists(QString::fromStdString(path)))
        {
            path.assign(ide);
        }

        if (ReadItemDefinitionFile(path.c_str()))
		{
            mainWindow->addFormattedMessage("\tЗагружено: %s", ide.c_str());
		}
		else
		{
            mainWindow->addFormattedMessage("\tНе удалось загрузить: %s", ide.c_str());
			ReturnValue = false;
		}
	}

	return ReturnValue;
}

bool BuildManager::ReadItemDefinitionFile(const char fname[])
{
	std::ifstream IDEFile(fname, std::ios::binary);

	if (IDEFile.is_open())
	{
		std::string thisLine;
		unsigned int inArea = 0;
		
		while(std::getline(IDEFile, thisLine))
		{
			if(thisLine.at(0) == '#') continue;
			thisLine.erase(std::find_if(thisLine.rbegin(), thisLine.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisLine.end());
			
			if(thisLine == "end")
			{
				inArea = 0;
			}
			else if(thisLine == "objs" || thisLine == "tobj")
			{
				inArea = 1;
			}
			else if(thisLine == "anim")
			{
				inArea = 2;
			}
			else if(thisLine == "weap")
			{
				inArea = 3;
			}
			else if(inArea != 0)
			{
				std::stringstream paramStream(thisLine);
				std::string thisParam;
				unsigned int index = 0;
				Modelid_Modelname_Struct thisObject;
				
				while(std::getline(paramStream, thisParam, ','))
				{
					thisParam.erase(thisParam.begin(), std::find_if(thisParam.begin(), thisParam.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
					thisParam.erase(std::find_if(thisParam.rbegin(), thisParam.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisParam.end());
				
					index++;
					
					try
					{
						switch(index)
						{
							case 1:
							{
								thisObject.modelid = static_cast<uint16_t>(std::stoi(thisParam));
								break;
							}
							case 2:
							{
								thisObject.ModelName = thisParam;
								std::transform(thisObject.ModelName.begin(), thisObject.ModelName.end(), thisObject.ModelName.begin(), ::tolower);
								break;
							}
							default:
							{
								if(index == (inArea + 3)) thisObject.DrawDist = std::stof(thisParam);
								break;
							}
						}
					}
					catch(std::invalid_argument e)
					{
						return false;
					}
					catch(std::out_of_range e)
					{
						return false;
					}
				}
				
                if(!thisObject.ModelName.length()) mainWindow->addFormattedMessage("\tОшибка: объект %d имеет пустое название.", thisObject.modelid);
				modelnameLookup.insert(thisObject.ModelName, thisObject.modelid);
				modelidLookup.emplace(std::pair<int, std::string>(thisObject.modelid, thisObject.ModelName));
				
				Modelid_Modelname_Vector.emplace_back(thisObject);
			}
		}
		
		return true;
	}
	
	return false;
}

void BuildManager::PrepareDatabaseStructures()
{
	bool ModelidMatch, ModelnameMatch, EntryFound;

	for(auto it = COLArray.begin(); it != COLArray.end(); )
	{
		auto model_it = modelidLookup.find(it->modelid);
		
		if(model_it == modelidLookup.end() || model_it->second != it->name)
		{
			ATree<int>::iterator name_it;
			
			if(modelnameLookup.find(it->name, name_it))
			{
				it->modelid = name_it->second;
			}
			else
			{
                mainWindow->addFormattedMessage("\tПредупреждение: коллизия %s не используется, игнорируется.", it->name.c_str());
				it = COLArray.erase(it);
				continue;
			}
		}
		
		++it;
	}

	//Alphasorting COL array
	std::sort(COLArray.begin(), COLArray.end(), [](ColModel const &a, ColModel const &b){ return a.modelid < b.modelid; });
	
	for(auto it = IPLArray.begin(); it != IPLArray.end(); )
	{
		auto coll_it = std::find_if(COLArray.begin(), COLArray.end(), [&it](ColModel &m) { return it->modelid == m.modelid; });
		if(coll_it == COLArray.end())
		{
			// No collision
			it = IPLArray.erase(it);
			continue;
		}
		
		++it;
	}
}

bool BuildManager::WriteBinaryFile(const char fname[])
{
	//Begin writing file
	std::ofstream ColAndreasBinaryfile(fname, std::ios::trunc | std::ios::binary);

	if (ColAndreasBinaryfile.is_open())
	{
		//Filetype in header (standard binary file format) "Otherwise you could give it any old file and it would load it."
		char FileExtension[5] = "cadf";
		ColAndreasBinaryfile.write((char*) &FileExtension, 4);
		
		//Version
        uint16_t dbVersion = 2;
		ColAndreasBinaryfile.write((char*) &dbVersion, 2);

		//Number of collision files
		uint16_t colCount = COLArray.size();
		ColAndreasBinaryfile.write((char*) &colCount, sizeof(colCount));
        mainWindow->addFormattedMessage("\tКол-во моделей: %u", colCount);
		
		//Number of item placment entries
		uint32_t iplCount = IPLArray.size();
		ColAndreasBinaryfile.write((char*) &iplCount, sizeof(iplCount));
        mainWindow->addFormattedMessage("\tКол-во IPL-размещений: %u", iplCount);

		for(ColModel &m : COLArray)
		{
			//Modelid
			ColAndreasBinaryfile.write((char*) &(m.modelid), sizeof(m.modelid));

			//number of collision spheres
			uint16_t sphereCount = static_cast<uint16_t>(m.num_spheres);
			ColAndreasBinaryfile.write((char*) &sphereCount, sizeof(sphereCount));

			//number of collision boxes
			uint16_t boxCount = static_cast<uint16_t>(m.num_boxes);
			ColAndreasBinaryfile.write((char*) &boxCount, sizeof(boxCount));

			//number of collision faces
			uint16_t faceCount = static_cast<uint16_t>(m.num_faces);
			ColAndreasBinaryfile.write((char*) &faceCount, sizeof(faceCount));

			//Sphere data
			for(int i = 0; i < m.num_spheres; ++i)
			{
				ColAndreasBinaryfile.write((char*) (m.spheres + i), sizeof(ColSphere));
			}
			
			//Box data
			for(int i = 0; i < m.num_boxes; ++i)
			{
				//Convert the data to save CPU ticks, when the ColAndreas plugin loads
				float CenterX = (m.boxes[i].min.x + m.boxes[i].max.x) / 2.0f;
				float CenterY = (m.boxes[i].min.y + m.boxes[i].max.y) / 2.0f;
				float CenterZ = (m.boxes[i].min.z + m.boxes[i].max.z) / 2.0f;

				float BoxsizeX = (m.boxes[i].max.x - m.boxes[i].min.x) / 2.0f;
				float BoxsizeY = (m.boxes[i].max.y - m.boxes[i].min.y) / 2.0f;
				float BoxsizeZ = (m.boxes[i].max.z - m.boxes[i].min.z) / 2.0f;

				ColAndreasBinaryfile.write((char*) &CenterX, 4);
				ColAndreasBinaryfile.write((char*) &CenterY, 4);
				ColAndreasBinaryfile.write((char*) &CenterZ, 4);

				ColAndreasBinaryfile.write((char*) &BoxsizeX, 4);
				ColAndreasBinaryfile.write((char*) &BoxsizeY, 4);
				ColAndreasBinaryfile.write((char*) &BoxsizeZ, 4);
			}

			//Face data
			for(int i = 0; i < m.num_faces; ++i)
			{
				ColAndreasBinaryfile.write((char*) m.faces[i].a, sizeof(Vertex));
				ColAndreasBinaryfile.write((char*) m.faces[i].b, sizeof(Vertex));
				ColAndreasBinaryfile.write((char*) m.faces[i].c, sizeof(Vertex));
			}
		}

		//IPL modelid, x,y,z x,y,z,w
		for(IPLObject &o : IPLArray)
		{
			o.Rotation.w *= -1.0f;
			
			ColAndreasBinaryfile.write((char*) &(o.modelid), sizeof(uint16_t));
			ColAndreasBinaryfile.write((char*) &(o.Position), sizeof(Vertex));
			ColAndreasBinaryfile.write((char*) &(o.Rotation), sizeof(IPLRot));
		}
		
        mainWindow->addFormattedMessage("\tРазмер файла: %.2f МБ", float(ColAndreasBinaryfile.tellp()) / (1024.0f * 1024.0f));
		
		return true;
	}
	return false;
}
