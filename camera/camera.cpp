#include "boost/chrono.hpp"
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include "camera.hpp"


using namespace cv;

///////////////////////
/// camera_t
///////////////////////

camera_t::camera_t(size_t obj_num, int idx, camera_connector_t *connector) :
        terminate_(0), objects_cnt_(obj_num) {
    connector_ = connector;
    ostringstream object_name;

    object_name << "camera " << idx;

    settings_.set_object_name(object_name.str());

    capture_ = new VideoCapture(idx);

    capture_->grab();


    obj_detects_ = new obj_detect_t *[objects_cnt_];

    for (size_t i = 0; i < objects_cnt_; ++i)
        obj_detects_[i] = new obj_detect_t(i);
}

int camera_t::FPS = 0;

void camera_t::loop() {
    Mat frame;
    chrono::time_point<chrono::system_clock> timeStart = chrono::system_clock::now();
    double timeAccum = 0;
    double fpsAccum = 0;
    int fpsCounter = 0;
    double deltaTime = 0;
    pos_t *pos = new pos_t[objects_cnt_];
    while (!capture_->isOpened())
        usleep(100);
    std::cout << "Starting camera loop\n";
    while (!terminate_) {
        usleep(10);
        chrono::duration<double> sec = (chrono::system_clock::now() - timeStart);
        deltaTime = sec.count();
        timeAccum += deltaTime;
        fpsAccum += deltaTime;
        timeStart = chrono::system_clock::now();
        if (timeAccum > 1. / 30.) {
            fpsCounter++;
            get_frame(frame);

            for (size_t i = 0; i < objects_cnt_; ++i) {
                obj_detects_[i]->detect(frame, pos[i]);

                // emit position_is_ready(pos[i].x, pos[i].y, i);
                connector_->position_is_ready(pos[i].x, pos[i].y, 0);
            }

            for (size_t i = 0; i < objects_cnt_; ++i) {
                obj_detects_[i]->draw_contours(frame);

                obj_detects_[i]->draw_position(frame, pos[i]);
            }


            connector_->frame_is_ready(frame);
            timeAccum = 0;
        }
        if (fpsAccum >= 1) {
            fpsAccum = 0;
            FPS = fpsCounter;
            fpsCounter = 0;
        }
    }

    delete[] pos;
}

void camera_t::stop() {
    terminate_ = 1;
}

void camera_t::get_frame(Mat &frame) {
    capture_->grab();
    capture_->retrieve(frame);
    frame.convertTo(frame, -1, intermediate_settings_.contrast_swr_, intermediate_settings_.brightness_swr_);
}

camera_t::~camera_t() {
    set_hardware_settings(settings_for_restore_);
}

void camera_t::import_settings(string const &file_name) {
    settings_.import_settings(file_name);

    set_hardware_settings(intermediate_settings_);
}

void camera_t::export_settings(ofstream &output_file) {
    settings_.export_settings(output_file);
}

void camera_t::set_hardware_settings(camera_settings_t const &settings) {
    capture_->set(CV_CAP_PROP_BRIGHTNESS, settings.brightness_hwr_);
    capture_->set(CV_CAP_PROP_CONTRAST, settings.contrast_hwr_);
    capture_->set(CV_CAP_PROP_SATURATION, settings.saturation_hwr_);
    capture_->set(CV_CAP_PROP_HUE, settings.hue_hwr_);
    capture_->set(CV_CAP_PROP_GAIN, settings.gain_hwr_);
    capture_->set(CV_CAP_PROP_EXPOSURE, settings.exposure_hwr_);
}

///////////////////////////////////////////////////////
/// setters / getters
///////////////////////////////////////////////////////

void camera_t::set_brightness_hwr(double val) {
    capture_->set(CV_CAP_PROP_BRIGHTNESS, val);
}

void camera_t::set_contrast_hwr(double val) {
    capture_->set(CV_CAP_PROP_CONTRAST, val);
}

void camera_t::set_saturation_hwr(double val) {
    capture_->set(CV_CAP_PROP_SATURATION, val);
}

void camera_t::set_hue_hwr(double val) {
    capture_->set(CV_CAP_PROP_HUE, val);
}

void camera_t::set_gain_hwr(double val) {
    capture_->set(CV_CAP_PROP_GAIN, val);
}

void camera_t::set_exposure_hwr(double val) {
    capture_->set(CV_CAP_PROP_EXPOSURE, val);
}

void camera_t::set_brightness_swr(double val) {
    intermediate_settings_.brightness_swr_ = val;
}

void camera_t::set_contrast_swr(double val) {
    intermediate_settings_.contrast_swr_ = val;
}

double camera_t::get_brightness_swr() const {
    return intermediate_settings_.brightness_swr_;
}

double camera_t::get_contrast_swr() const {
    return intermediate_settings_.contrast_swr_;
}

double camera_t::get_brightness_hwr() const {
    return intermediate_settings_.brightness_hwr_;
}

double camera_t::get_contrast_hwr() const {
    return intermediate_settings_.contrast_hwr_;
}

double camera_t::get_saturation_hwr() const {
    return intermediate_settings_.saturation_hwr_;
}

double camera_t::get_hue_hwr() const {
    return intermediate_settings_.hue_hwr_;
}

double camera_t::get_gain_hwr() const {
    return intermediate_settings_.gain_hwr_;
}

double camera_t::get_exposure_hwr() const {
    return intermediate_settings_.exposure_hwr_;
}

///////////////////////
/// 
///////////////////////

obj_detect_t::obj_detect_t(size_t idx) :
        start_recording_track_(0), draw_track_(0), draw_mesh_(0) {
    ostringstream object_name;
    object_name << "object track " << idx;

    settings_.set_object_name(object_name.str());

    settings_.add_setting(&min_h_, "minimum hue");
    settings_.add_setting(&min_s_, "minimum saturation");
    settings_.add_setting(&min_v_, "minimum value");
    settings_.add_setting(&max_h_, "maximum hue");
    settings_.add_setting(&max_s_, "maximum saturation");
    settings_.add_setting(&max_v_, "maximum value");

    settings_.add_setting(&min_obj_size_, "minimum object size");
    settings_.add_setting(&max_obj_size_, "maximum object size");
}

obj_detect_t::~obj_detect_t() {
}

void obj_detect_t::detect(Mat &origin_frame, pos_t &position) {
    Mat frame
    , threshold_frm;

    origin_frame.copyTo(frame);

    ostringstream fps_string;

    chrono::time_point<chrono::system_clock> time_point;

    time_point = chrono::system_clock::now();


    Point2d pos, cur_position;

    double area;

    size_t positions_find;

    frame.copyTo(threshold_frm);

    cvtColor(threshold_frm, threshold_frm, CV_BGR2HSV);

    inRange(threshold_frm, Scalar(min_h_, min_s_, min_v_), Scalar(max_h_, max_s_, max_v_), threshold_frm);

    findContours(threshold_frm, contours_, hierarchy_, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

    cur_position.x = cur_position.y = 0;

    positions_find = 0;

    if (hierarchy_.size()) {
        for (int index = 0; index >= 0; index = hierarchy_[index][0]) {
            Moments moment = moments((cv::Mat) contours_[index]);
            area = moment.m00;

            if (area > min_obj_size_ && area < max_obj_size_) {
                cur_position.x += moment.m10 / area;
                cur_position.y += moment.m01 / area;

                positions_find++;
            }
        }
    }

    if (positions_find) {
        cur_position.x /= (double) (positions_find * 1);
        cur_position.y /= (double) (positions_find * 1);

        pos = cur_position;

        circle(frame, pos, 10, Scalar(0, 0, 255), 2);
    }

    fps_string << "FPS: " << camera_t::FPS;

    putText(origin_frame, fps_string.str(), Point2i(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

    fps_string.str("");
    fps_string.clear();

    fps_string << "Position: " << "[" << position.x << ", " << position.y << "]";

    putText(origin_frame, fps_string.str(), Point2i(10, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

    fps_string.str("");
    fps_string.clear();
}

void obj_detect_t::draw_contours(Mat &frame) {
    for (int index = 0; index >= 0; index = hierarchy_[index][0])
        drawContours(frame, contours_, index, Scalar(0, 255, 0), 3, 8, hierarchy_);
}

void obj_detect_t::draw_position(Mat &frame, pos_t position) {
    circle(frame, Point2d(position.x, position.y), 10, Scalar(0, 0, 255), 2);
}

void obj_detect_t::draw_track() {
    draw_track_ = 1;
}

void obj_detect_t::stop_draw_track() {
    draw_track_ = 0;
}

void obj_detect_t::draw_mesh() {
    draw_mesh_ = 1;
}

void obj_detect_t::stop_draw_mesh() {
    draw_mesh_ = 0;
}

void obj_detect_t::start_recording_track() {
    start_recording_track_ = 1;
}

void obj_detect_t::stop_recording_track() {
    start_recording_track_ = 0;
}

void obj_detect_t::import_settings(string const &file_name) {
    settings_.import_settings(file_name);
}

void obj_detect_t::export_settings(ofstream &output_file) {
    settings_.export_settings(output_file);
}

/////////////////////////////////////////
/// setters / getters
/////////////////////////////////////////

void obj_detect_t::set_max_v(int max_v) {
    this->max_v_ = max_v;
}

void obj_detect_t::set_min_v(int min_v) {
    this->min_v_ = min_v;
}

void obj_detect_t::set_max_s(int max_s) {
    this->max_s_ = max_s;
}

void obj_detect_t::set_min_s(int min_s) {
    this->min_s_ = min_s;
}

void obj_detect_t::set_max_h(int max_h) {
    this->max_h_ = max_h;
}

void obj_detect_t::set_min_h(int min_h) {
    this->min_h_ = min_h;
}

int obj_detect_t::get_max_v() const {
    return max_v_;
}

int obj_detect_t::get_min_v() const {
    return min_v_;
}

int obj_detect_t::get_max_s() const {
    return max_s_;
}

int obj_detect_t::get_min_s() const {
    return min_s_;
}

int obj_detect_t::get_max_h() const {
    return max_h_;
}

int obj_detect_t::get_min_h() const {
    return min_h_;
}

void obj_detect_t::set_min_obj_size(int min_size) {
    min_obj_size_ = min_size;
}

void obj_detect_t::set_max_obj_size(int max_size) {
    max_obj_size_ = max_size;
}

int obj_detect_t::get_min_obj_size() const {
    return min_obj_size_;
}

int obj_detect_t::get_max_obj_size() const {
    return max_obj_size_;
}
