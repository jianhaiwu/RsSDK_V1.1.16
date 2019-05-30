#pragma once
#include <set>

class sync_playback : public base_instance {
public:
    explicit sync_playback(void* parent)
        : base_instance(parent, SYNC_PLAYBACK_INSTANCE_TYPE, "sync_playback")
        , sync_play_handle_(NULL) 
    {

    }

    ~sync_playback() {
        stop();
    }

    rs_error_code start(const sync_playback_param* param) {
        stop();

        sync_play_handle_ = RSPlayCreatePlayInstance(PLAY_SYNC_TYPE);
        base_play_handle_ = sync_play_handle_;
        if (sync_play_handle_ && param && param->sess && param->sess_count) {
            for (int i = 0; i < param->sess_count; i++) {
                base_instance* sess = (base_instance*)(param->sess[i]);
                if (!sess || !sess->get_play_handle()) {
                    sdk_wrapper_log("[:] call <sync_playback start> result:fail\n");
                    return rs_fail;
                }
                
                RSPlayAddSyncPlayInstance(sync_play_handle_, sess->get_play_handle());
                if (sess->get_device()) {
                    if (sess->start_net() != rs_success) {
                        sdk_wrapper_log("[:] call <sync_playback start> result:fail\n");
                        return rs_fail;
                    }
                }

                sync_info_.push_back(sess);
                sync_info_dynamic_.insert(sess);
            }
            RSPlayStartPlay(sync_play_handle_);
            sdk_wrapper_log("[:] call <sync_playback start> result:success\n");
            return rs_success;
        }
        sdk_wrapper_log("[:] call <sync_playback start> result:fail\n");
        return rs_fail;
    }

    rs_error_code stop() {
        return stop_internal(false);
    }

    uint64_t convert_utc_to_microsecond(const char* param) {
        int year, month, day, hour, minute, second;
        sscanf(param, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
        month--;
        day--;
        
        int days = 0;
        for (int begin_year = 1970; begin_year < year; begin_year++) {
            if ((begin_year%4 == 0 && begin_year%100 != 0) || begin_year%400 == 0)
                days += 366;
            else
                days += 365;
        }
        
        static int global_rs_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        for (int begin_month = 0; begin_month < month; begin_month++) {
            if (begin_month == 1 && ((year%4 == 0 && year%100 != 0) || year%400 == 0))
                days += 29;
            else
                days += global_rs_month[begin_month];
        }
        days += day;
        
        uint64_t secons = (days*24*60*60 + hour*60*60 + minute*60 + second);
        secons *= 1000000;
        return secons;
    }
    
    rs_error_code seek_by_time(const char* param) {
        if (sync_play_handle_) {
            for (size_t i = 0; i < sync_info_.size(); i++)
                sync_info_.at(i)->seek_by_time(param);
            return rs_success;
        }
        return rs_fail;
    }

    rs_error_code set_play_mode(const char* param) {
        if (sync_play_handle_) {
            std::string err = "";
            Json::CharReaderBuilder reader;
            Json::Value value;
            if(!Json::parseFromStream(reader, param, &value, &err))
                return rs_parse_json_error;

            int play_mode = -1;
            std::string type = value["operation type"].asString();
            if (type == "normal") {
                play_mode = PLAY_MODE_NORMAL;
            } else if (type == "pause") {
                play_mode = PLAY_MODE_PAUSE;
            } else if (type == "single frame") {
                play_mode = PLAY_MODE_SINGLE_FRAME;
            } else if (type == "16 times slower") {
                play_mode = PLAY_MODE_SLOW16;
            } else if (type == "8 times slower") {
                play_mode = PLAY_MODE_SLOW8;
            } else if (type == "4 times slower") {
                play_mode = PLAY_MODE_SLOW4;
            } else if (type == "2 times slower") {
                play_mode = PLAY_MODE_SLOW;
            } else if (type == "2 times faster") {
                play_mode = PLAY_MODE_FAST_FORWARD2;
            } else if (type == "4 times faster") {
                play_mode = PLAY_MODE_FAST_FORWARD4;
            } else if (type == "8 times faster") {
                play_mode = PLAY_MODE_FAST_FORWARD8;
            } else if (type == "16 times faster") {
                play_mode = PLAY_MODE_FAST_FORWARD16;
            } else {
                return rs_fail;
            }

            RSPlaySetPlayMode(sync_play_handle_, play_mode, 0);
            return rs_success;
        }
        return rs_fail;
    }

    rs_error_code add_session(base_instance* instance) { 
        if (instance && instance->get_device()) {
            if (sync_play_handle_) {
                RSPlayAddSyncPlayInstance(sync_play_handle_, instance->get_play_handle());
                instance->start_net();
                sync_info_.push_back(instance);
                sync_info_dynamic_.insert(instance);
                return rs_success; 
            } else {
                sync_playback_param param = {0};
                param.sess = (session_id*)(&instance);
                param.sess_count = 1;
                return start(&param);
            }
        }
        return rs_fail;
    }

    rs_error_code del_session(base_instance* instance) { 
        if (instance && instance->get_device()) {
            if (sync_info_dynamic_.size() > 1) {
                assert(sync_play_handle_);
                RSPlayDelSyncPlayInstance(sync_play_handle_, instance->get_play_handle());
                std::set<base_instance*>::iterator itor = sync_info_dynamic_.find(instance);
                if (itor != sync_info_dynamic_.end())
                    sync_info_dynamic_.erase(itor);
                return rs_success;
            } else {
                return stop_internal(true);
            }
        }
        return rs_fail;
    }

private:
    rs_error_code stop_internal(bool call_internal) {
        if (!base_running_ && !sync_play_handle_)
            return rs_success;

        base_running_ = false;

        for (size_t i = 0; i < sync_info_.size(); i++) {
            base_instance* sess = sync_info_.at(i);
            sess->stop_net();
        }

        if (sync_play_handle_) {
            RSPlayStopPlay(sync_play_handle_);
            for (size_t i = 0; i < sync_info_.size(); i++) {
                base_instance* sess = sync_info_.at(i);
                sess->stop();
                sess->remote_self_from_device_list();
                if (!call_internal)
                    delete sess;
            }
            RSPlayDestroyPlayInstance(sync_play_handle_);
            sync_play_handle_ = NULL;
            base_play_handle_ = NULL;
        }

        if (!call_internal)
            sync_info_.clear();
        sync_info_dynamic_.clear();

        sdk_wrapper_log("[:] call <sync_playback stop> result:success\n");
        return rs_success;
    }

private:
    RSPlayHandle                sync_play_handle_;
    std::vector<base_instance*> sync_info_;
    std::set<base_instance*>    sync_info_dynamic_;
};
