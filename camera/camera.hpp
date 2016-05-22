#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include <opencv2/opencv.hpp>

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/features2d.hpp>
//


#include "camera_connector.hpp"
#include "settings.hpp"
using namespace cv;

class obj_detect_t;

class camera_t
{

   
public:
   camera_t(size_t obj_num, int idx, camera_connector_t* connector);
   ~camera_t();
   
   void loop();

    static int FPS;
   void stop();
   
   void import_settings( string const & file_name );
   void export_settings( ofstream & output_file );

   
   void set_brightness_hwr( double val );
   void set_contrast_hwr  ( double val );
   void set_saturation_hwr( double val );
   void set_hue_hwr       ( double val );
   void set_gain_hwr      ( double val );
   void set_exposure_hwr  ( double val );

   double get_brightness_hwr() const;
   double get_contrast_hwr  () const;
   double get_saturation_hwr() const;
   double get_hue_hwr       () const;
   double get_gain_hwr      () const;
   double get_exposure_hwr  () const;

   void set_brightness_swr( double val );
   void set_contrast_swr  ( double val );

   double get_brightness_swr() const;
   double get_contrast_swr  () const;

private:
   camera_connector_t* connector_;
   obj_detect_t ** obj_detects_;
   
   size_t objects_cnt_;
   
   struct camera_settings_t
   {
       camera_settings_t() :
               brightness_swr_(2), contrast_swr_(2) { }
      double   brightness_hwr_
             , contrast_hwr_
             , saturation_hwr_
             , hue_hwr_
             , gain_hwr_
             , exposure_hwr_;

      double   brightness_swr_
             , contrast_swr_;
   };
   
   VideoCapture * capture_;
   
   int terminate_;
      
   camera_settings_t  settings_for_restore_
                    , intermediate_settings_;

   settings_t settings_;
   
private:
   void set_hardware_settings( camera_settings_t const & settings );

   void get_frame( Mat & frame );
};

class track_t;
class pos_t;

class obj_detect_t
{
   
public:
   obj_detect_t( size_t idx = 0 );
   ~obj_detect_t();

    void detect(Mat &frame, pos_t &position);
   
   void draw_contours( Mat & frame );
   void draw_position( Mat & frame, pos_t position );

   void stop();

   void start_recording_track();
   void stop_recording_track();
   
   void draw_track     ();
   void stop_draw_track();

   void draw_mesh     ();
   void stop_draw_mesh();
   
   void import_settings( string const & file_name );
   void export_settings( ofstream & output_file );
   
   void set_max_v( int max_v );
   void set_min_v( int min_v );
   void set_max_s( int max_s );
   void set_min_s( int min_s );
   void set_max_h( int max_h );
   void set_min_h( int min_h );

   int get_min_h() const;
   int get_max_h() const;
   int get_min_s() const;
   int get_max_s() const;
   int get_min_v() const;
   int get_max_v() const;
   
   void set_min_obj_size( int max_h );
   void set_max_obj_size( int min_h );

   int get_min_obj_size() const;
   int get_max_obj_size() const;
   
private:
   size_t   min_h_ = 0, max_h_ = 255
          , min_s_ = 0, max_s_ = 255
          , min_v_ = 0, max_v_ = 255
          , min_obj_size_ = 0, max_obj_size_ = 10000;
   
   int   start_recording_track_
       , draw_track_
       , draw_mesh_;
   
   vector< vector<Point> > contours_;
   vector< Vec4i >         hierarchy_;
   
   settings_t settings_;
};

struct pos_t
{
   pos_t( double x_val = 0, double y_val = 0 ) :
        x(x_val)
      , y(y_val)
   {}

   pos_t & operator = ( pos_t const & pos )
   {
      this->x = pos.x;
      this->y = pos.y;
   }

   double x, y;
};

