#include "settings.hpp"

settings_t::settings_t( string const & object_name ) :
   object_name_(object_name)
{
}

void settings_t::set_object_name( string const & object_name )
{
   object_name_ = object_name;
}

void settings_t::import_settings( string const & file_name )
{
   Node const objects = LoadFile(file_name);

   for (size_t obj_idx = 0; obj_idx < objects.size(); ++obj_idx)
   {
      if (objects[obj_idx]["object name"].as<string>().compare(object_name_))
         continue;
      
      Node const config = objects[obj_idx]["settings"];
      
      string setting_name;
      
      cout << objects[obj_idx]["object name"].as<string>() << endl;
      
      for (size_t i = 0; i < config.size(); ++i)
      {
         setting_name = (config[i])["name"].as<string>();

         for (auto it = settings_map_dbl_.begin(); it != settings_map_dbl_.end(); ++it)
         {
            if (!it->first.compare(setting_name))
               *(it->second) = (config[i])["value"].as<double>();
         }

         for (auto it = settings_map_ui_.begin(); it != settings_map_ui_.end(); ++it)
         {
            if (!it->first.compare(setting_name))
               *(it->second) = (config[i])["value"].as<size_t>();
         }
      }
      
      break;
   }
}

void settings_t::export_settings( ofstream & output_file )
{
   Emitter emitter;
   
   emitter << BeginSeq << BeginMap << Key << "object name" << Value << object_name_ << Key << "settings";
   
   emitter << BeginSeq;
   
   for (auto it = settings_map_ui_.begin(); it != settings_map_ui_.end(); ++it)
   {
      emitter << BeginMap
              << Key << "name"  << Value << it->first << Key << "value" << Value << *(it->second) 
              << EndMap;
   }
   
   for (auto it = settings_map_dbl_.begin(); it != settings_map_dbl_.end(); ++it)
   {
      emitter << BeginMap
              << Key << "name"  << Value << it->first << Key << "value" << Value << *(it->second) 
              << EndMap;
   }

   emitter << EndSeq;
   
   emitter << EndMap << EndSeq << Newline << Newline;
   
   output_file << emitter.c_str();
}

void settings_t::add_setting( size_t * setting, string const & setting_name )
{
   settings_map_ui_.insert(make_pair(setting_name, setting));
}

void settings_t::add_setting( double * setting, string const & setting_name )
{
   settings_map_dbl_.insert(make_pair(setting_name, setting));
}
