#include <iostream>
#include <fstream>

#include "gui.h"

gui::gui(size_t cam_num, QWidget *parent) :
        QMainWindow(parent), ui_(new Ui::gui), curr_cam_idx_(0) {
    gui_.reset(this);

    ui_->setupUi(this);

    ui_->retranslateUi(this);

//   cameras_num_ = video_capture->cameras_num();
    cameras_num_ = cam_num;

    ot_cam_menu_.reset(new camera_menu_ptr_t[cameras_num_]);

    for (size_t i = 0; i < cameras_num_; ++i) {
        ot_cam_menu_.get()[i].reset(new camera_menu_t(ui_->object_track_settings, ui_->view_menu, gui_, i));

        connect(ot_cam_menu_.get()[i].get(), SIGNAL(make_this_camera_current(size_t)), gui_.get(),
                SLOT(set_current_cam(size_t)));
    }

//   set_current_cam(0);

    connect(ui_->ot_start_recording, SIGNAL(triggered()), this, SLOT(call_record_track()));
    connect(ui_->ot_stop_recording, SIGNAL(triggered()), this, SLOT(call_stop_record_track()));
    connect(ui_->v_show_track, SIGNAL(triggered()), this, SLOT(call_show_track()));
    connect(ui_->v_show_mesh, SIGNAL(triggered()), this, SLOT(call_show_mesh()));
    connect(ui_->f_import_settings, SIGNAL(triggered()), this, SLOT(import_settings()));
    connect(ui_->f_export_settings, SIGNAL(triggered()), this, SLOT(export_settings()));

//   connect(ui_->rm_start_robot, SIGNAL(triggered()), robot_control_.get(), SLOT(start_robot()));
//   connect(ui_->rm_stop_robot, SIGNAL(triggered()), robot_control_.get(), SLOT(stop_robot()));
}

Q_SLOT void gui::redraw(QImage image) {
    ui_->label_pic->setPixmap(QPixmap::fromImage(image));
}

void gui::closeEvent(QCloseEvent *event) {
//   for (size_t i = 0; i < cameras_num_; ++i)
//      obj_tracks_.get()[i]->stop();
//      
//   robot_control_->stop();
}

Q_SLOT void gui::call_record_track() {
//   for (size_t i = 0; i < cameras_num_; ++i)
//      obj_tracks_.get()[i]->start_recording_track();

    std::cout << "Start recording track" << std::endl;
}

Q_SLOT void gui::call_stop_record_track() {
//   for (size_t i = 0; i < cameras_num_; ++i)
//      obj_tracks_.get()[i]->stop_recording_track();

    std::cout << "Stop recording track" << std::endl;
}

Q_SLOT void gui::call_show_track() {
//   if (ui_->v_show_track->isChecked())
//   {
//      for (size_t i = 0; i < cameras_num_; ++i)
//         obj_tracks_.get()[i]->draw_track();
//   }
//   else
//      for (size_t i = 0; i < cameras_num_; ++i)
//         obj_tracks_.get()[i]->stop_draw_track();
}

Q_SLOT void gui::call_show_mesh() {
//   if (ui_->v_show_mesh->isChecked())
//   {
//      for (size_t i = 0; i < cameras_num_; ++i)
//         obj_tracks_.get()[i]->draw_mesh();
//   }
//   else
//      for (size_t i = 0; i < cameras_num_; ++i)
//         obj_tracks_.get()[i]->stop_draw_mesh();
}

Q_SLOT void gui::import_settings() {
    string output_file_name("settings.yml");

//   for (size_t i = 0; i < cameras_num_; ++i)
//   {
//      obj_tracks_.get()[i]->import_settings(output_file_name);
//      video_capture_->get_camera(1)->import_settings(output_file_name);
//
//      ot_cam_menu_.get()[i]->op_win_->update_values();
//      ot_cam_menu_.get()[i]->cs_win_->update_values();
//   }
}

Q_SLOT void gui::export_settings() {
    ofstream output_file("settings.yml");

//   for (size_t i = 0; i < cameras_num_; ++i)
//   {
//      obj_tracks_.get()[i]->export_settings(output_file);
//      video_capture_->get_camera(i)->export_settings(output_file);
//   }
}

Q_SLOT void gui::set_current_cam(size_t cam_idx) {
    curr_cam_idx_ = cam_idx;

//   for (size_t i = 0; i < cam_idx; ++i)
//   {
//      disconnect(obj_tracks_.get()[i].get(), SIGNAL(frame_is_ready(QImage)), gui_.get(), SLOT(redraw(QImage)));
//      ot_cam_menu_.get()[i]->is_current(0);
//   }
//
//   for (size_t i = cam_idx + 1; i < cameras_num_; ++i)
//   {
//      disconnect(obj_tracks_.get()[i].get(), SIGNAL(frame_is_ready(QImage)), gui_.get(), SLOT(redraw(QImage)));
//      ot_cam_menu_.get()[i]->is_current(0);
//   }
//   
//   ot_cam_menu_.get()[cam_idx]->is_current(1);
//   connect(obj_tracks_.get()[cam_idx].get(), SIGNAL(frame_is_ready(QImage)), gui_.get(), SLOT(redraw(QImage)));
}

gui::~gui() {
}

camera_menu_t::camera_menu_t(QWidget *ot_parent, QMenu *view_parent, shared_ptr<QMainWindow> gui, size_t idx) :
        idx_(idx) {
    camera_menu_.reset(new QMenu(ot_parent));

    ostringstream camera_object_name;
    camera_object_name << "camera_menu_" << idx;

    camera_menu_->setObjectName(QString::fromUtf8(camera_object_name.str().c_str()));
    ot_parent->addAction(camera_menu_->menuAction());

    ostringstream obj_params_name;
    obj_params_name << "object_params_" << idx;

    object_params_.reset(new QAction(gui.get()));
    object_params_->setObjectName(QString::fromUtf8(obj_params_name.str().c_str()));

    ostringstream camera_settings_name;
    camera_settings_name << "camera_settings_" << idx;

    camera_settings_.reset(new QAction(gui.get()));
    camera_settings_->setObjectName(QString::fromUtf8(obj_params_name.str().c_str()));

    camera_menu_->addAction(object_params_.get());
    camera_menu_->addAction(camera_settings_.get());

    object_params_->setText(QApplication::translate("gui", "Object parameters", 0, QApplication::UnicodeUTF8));
    camera_settings_->setText(QApplication::translate("gui", "Camera settings", 0, QApplication::UnicodeUTF8));

    ostringstream camera_menu_title;
    camera_menu_title << "Camera " << idx;

    camera_menu_->setTitle(
            QApplication::translate("gui", camera_menu_title.str().c_str(), 0, QApplication::UnicodeUTF8));

    op_win_.reset();
    cs_win_.reset();

    connect(object_params_.get(), SIGNAL(triggered()), this, SLOT(call_object_params_win()));
    connect(camera_settings_.get(), SIGNAL(triggered()), this, SLOT(call_camera_settings_win()));

    // view menu
    view_camera_.reset(new QAction(gui.get()));

    ostringstream view_camera_menu_name;
    view_camera_menu_name << "view_camera_" << idx;

    view_camera_->setObjectName(QString::fromUtf8(view_camera_menu_name.str().c_str()));
    view_camera_->setCheckable(true);

    view_parent->addAction(view_camera_.get());

    view_camera_->setText(
            QApplication::translate("gui", camera_menu_title.str().c_str(), 0, QApplication::UnicodeUTF8));

    connect(view_camera_.get(), SIGNAL(triggered()), this, SLOT(call_show_camera()));
}

camera_menu_t::~camera_menu_t() {
    if (op_win_->isEnabled())
        op_win_->close();

    if (cs_win_->isEnabled())
        cs_win_->close();

    disconnect(0, 0, this, 0);
}

Q_SLOT void camera_menu_t::call_object_params_win() {
    op_win_->show();
}

Q_SLOT void camera_menu_t::call_camera_settings_win() {
    cs_win_->show();
}

Q_SLOT void camera_menu_t::call_show_camera() {
    if (view_camera_->isChecked())
            emit make_this_camera_current(idx_);
}

void camera_menu_t::is_current(int is_current) {
    view_camera_->setChecked(is_current);
}

///////////////////////////////////////
/// color range window
///////////////////////////////////////

object_params::object_params(QWidget *parent) :
        QMainWindow(parent), object_params_(new Ui::object_params) {
    object_params_->setupUi(this);

    object_params_->retranslateUi(this);

    connect(object_params_->hue_min, SIGNAL(valueChanged(int)), this, SLOT(set_h_min(int)));
    connect(object_params_->hue_max, SIGNAL(valueChanged(int)), this, SLOT(set_h_max(int)));
    connect(object_params_->saturation_min, SIGNAL(valueChanged(int)), this, SLOT(set_s_min(int)));
    connect(object_params_->saturation_max, SIGNAL(valueChanged(int)), this, SLOT(set_s_max(int)));
    connect(object_params_->value_min, SIGNAL(valueChanged(int)), this, SLOT(set_v_min(int)));
    connect(object_params_->value_max, SIGNAL(valueChanged(int)), this, SLOT(set_v_max(int)));
    connect(object_params_->obj_size_min, SIGNAL(valueChanged(int)), this, SLOT(set_obj_size_min(int)));
    connect(object_params_->obj_size_max, SIGNAL(valueChanged(int)), this, SLOT(set_obj_size_max(int)));
}

object_params::~object_params() {
}

void object_params::update_values() {
//   object_params_->hue_min->setValue(obj_track_->get_min_h());
//   object_params_->hue_max->setValue(obj_track_->get_max_h());
//   object_params_->saturation_min->setValue(obj_track_->get_min_s());
//   object_params_->saturation_max->setValue(obj_track_->get_max_s());
//   object_params_->value_min->setValue(obj_track_->get_min_v());
//   object_params_->value_max->setValue(obj_track_->get_max_v());
//   object_params_->value_min->setValue(obj_track_->get_min_v());
//   object_params_->value_max->setValue(obj_track_->get_max_v());
//   
//   object_params_->obj_size_min->setValue(obj_track_->get_min_obj_size());
//   object_params_->obj_size_max->setValue(obj_track_->get_max_obj_size());
}

Q_SLOT void object_params::set_h_min(int val) {
//   if (val > obj_track_->get_max_h())
//      object_params_->hue_min->setValue(obj_track_->get_max_h());
//   else
//      obj_track_->set_min_h(val);
}

Q_SLOT void object_params::set_h_max(int val) {
//   if (val < obj_track_->get_min_h())
//      object_params_->hue_max->setValue(obj_track_->get_min_h());
//   else
//      obj_track_->set_max_h(val);
}

Q_SLOT void object_params::set_s_min(int val) {
//   if (val > obj_track_->get_max_s())
//      object_params_->saturation_min->setValue(obj_track_->get_max_s());
//   else
//      obj_track_->set_min_s(val);
}

Q_SLOT void object_params::set_s_max(int val) {
//   if (val < obj_track_->get_min_s())
//      object_params_->saturation_max->setValue(obj_track_->get_min_s());
//   else
//      obj_track_->set_max_s(val);
}

Q_SLOT void object_params::set_v_min(int val) {
//   if (val > obj_track_->get_max_v())
//      object_params_->value_min->setValue(obj_track_->get_max_v());
//   else
//      obj_track_->set_min_v(val);
}

Q_SLOT void object_params::set_v_max(int val) {
//   if (val < obj_track_->get_min_v())
//      object_params_->value_max->setValue(obj_track_->get_min_v());
//   else
//      obj_track_->set_max_v(val);
}

Q_SLOT void object_params::set_obj_size_min(int val) {
//   if (val > obj_track_->get_max_obj_size())
//      object_params_->obj_size_min->setValue(obj_track_->get_max_obj_size());
//   else
//      obj_track_->set_min_obj_size(val);
}

Q_SLOT void object_params::set_obj_size_max(int val) {
//   if (val < obj_track_->get_min_obj_size())
//      object_params_->obj_size_max->setValue(obj_track_->get_min_obj_size());
//   else
//      obj_track_->set_max_obj_size(val);
}

///////////////////////////////////////
/// color range window
///////////////////////////////////////

camera_settings::camera_settings(QWidget *parent) :
        QMainWindow(parent), camera_settings_(new Ui::camera_settings) {
    camera_settings_->setupUi(this);

    camera_settings_->retranslateUi(this);

    update_values();

    connect(camera_settings_->brightness_hardware, SIGNAL(valueChanged(double)), this,
            SLOT(set_brightness_hardware(double)));
    connect(camera_settings_->contrast_hardware, SIGNAL(valueChanged(double)), this,
            SLOT(set_contrast_hardware(double)));
    connect(camera_settings_->saturation_hardware, SIGNAL(valueChanged(double)), this,
            SLOT(set_saturation_hardware(double)));
    connect(camera_settings_->hue_hardware, SIGNAL(valueChanged(double)), this, SLOT(set_hue_hardware(double)));
    connect(camera_settings_->gain_hardware, SIGNAL(valueChanged(double)), this, SLOT(set_gain_hardware(double)));
    connect(camera_settings_->exposure_hardware, SIGNAL(valueChanged(double)), this,
            SLOT(set_exposure_hardware(double)));
    connect(camera_settings_->brightness_software, SIGNAL(valueChanged(double)), this,
            SLOT(set_brightness_software(double)));
    connect(camera_settings_->contrast_software, SIGNAL(valueChanged(double)), this,
            SLOT(set_contrast_software(double)));
}

camera_settings::~camera_settings() {
}

void camera_settings::update_values() {
//   camera_settings_->brightness_hardware->setValue(camera_->get_brightness_hwr());
//   camera_settings_->contrast_hardware->setValue(camera_->get_contrast_hwr());
//   camera_settings_->exposure_hardware->setValue(camera_->get_exposure_hwr());
//   camera_settings_->gain_hardware->setValue(camera_->get_gain_hwr());
//   camera_settings_->hue_hardware->setValue(camera_->get_hue_hwr());
//   camera_settings_->saturation_hardware->setValue(camera_->get_saturation_hwr());
//
//   camera_settings_->brightness_software->setValue(camera_->get_brightness_swr());
//   camera_settings_->contrast_software->setValue(camera_->get_contrast_swr());
}

Q_SLOT void camera_settings::set_brightness_hardware(double val) {
//   camera_->set_brightness_hwr(val);
}

Q_SLOT void camera_settings::set_contrast_hardware(double val) {
//   camera_->set_contrast_hwr(val);
}

Q_SLOT void camera_settings::set_saturation_hardware(double val) {
//   camera_->set_saturation_hwr(val);
}

Q_SLOT void camera_settings::set_hue_hardware(double val) {
//   camera_->set_hue_hwr(val);
}

Q_SLOT void camera_settings::set_gain_hardware(double val) {
//   camera_->set_gain_hwr(val);
}

Q_SLOT void camera_settings::set_exposure_hardware(double val) {
//   camera_->set_exposure_hwr(val);
}

Q_SLOT void camera_settings::set_brightness_software(double val) {
//   camera_->set_brightness_swr(val);
}

Q_SLOT void camera_settings::set_contrast_software(double val) {
//   camera_->set_contrast_swr(val);
}

void gui::set_gui_connector(long long gui) {
    connect((gui_connector_t *) gui, SIGNAL(frame_received(QImage)), this, SLOT(redraw(QImage)));
}
