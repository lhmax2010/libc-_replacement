/*
 * Calendar Service
 *
 * Copyright (c) 2012 - 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __TIZEN_SOCIAL_CALENDAR_INTERNAL_H__
#define __TIZEN_SOCIAL_CALENDAR_INTERNAL_H__

calendar_time_s*  cal_caltime_create(void);

void cal_caltime_destroy(calendar_time_s *caltime);

calendar_time_type_e cal_caltime_get_local_type(calendar_time_s *caltime);

int cal_caltime_set_local_type(calendar_time_s *caltime, calendar_time_type_e type);

long long int cal_caltime_get_local_utime(calendar_time_s *caltime);

int cal_caltime_set_local_utime(calendar_time_s *caltime, long long int utime);

int cal_caltime_get_local_year(calendar_time_s *caltime);

int cal_caltime_set_local_year(calendar_time_s *caltime, int year);

int cal_caltime_get_local_month(calendar_time_s *caltime);

int cal_caltime_set_local_month(calendar_time_s *caltime, int month);

int cal_caltime_get_local_mday(calendar_time_s *caltime);

int cal_caltime_set_local_mday(calendar_time_s *caltime, int mday);

int cal_caltime_get_local_hour(calendar_time_s *caltime);

int cal_caltime_set_local_hour(calendar_time_s *caltime, int hour);

int cal_caltime_get_local_minute(calendar_time_s *caltime);

int cal_caltime_set_local_minute(calendar_time_s *caltime, int minute);

int cal_caltime_get_local_second(calendar_time_s *caltime);

int cal_caltime_set_local_second(calendar_time_s *caltime, int second);



#endif /*__TIZEN_SOCIAL_CALENDAR_INTERNAL_H__ */
