#pragma once

#include <map>
#include <fstream>
#include <string>
#include <iostream>

#include <yaml-cpp/yaml.h>

using namespace std;
using namespace YAML;

class settings_t
{
public:
   settings_t( string const & object_name = "" );
   
   void set_object_name( string const & object_name );

   void import_settings( string const & file_name );
   void export_settings( ofstream & output_file );

   void add_setting( size_t * setting, string const & setting_name );
   void add_setting( double * setting, string const & setting_name );

private:
   typedef map<string, double *> map_dbl_t;
   typedef map<string, size_t *> map_ui_t;

   map_dbl_t settings_map_dbl_;
   map_ui_t  settings_map_ui_;
   
   string object_name_;
};
