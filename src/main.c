/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tizen.h>
#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <system_settings.h>
#include <dlog.h>

#define PADDING_END "padding_end"
#define TIMER_INTERVAL 10.0
#define DEBUG_OVERRIDE_ENABLED 0
#define DEBUG_OVERRIDE_DAY 1
#define DEBUG_OVERRIDE_HOUR 23
#define DEBUG_OVERRIDE_MIN 59
#define DEBUG_OVERRIDE_SEC 30

static Ecore_Timer *debug_clock_timer = NULL;
static struct tm debug_clock;

typedef struct period {
	int majornum;
	int minornum;
	char name[40];
	char room[16];
	//struct tm start_time;
	//struct tm end_time;
	int start_hour;
	int start_min;
	int end_hour;
	int end_min;
	int duration_in_minutes;
} period_s;

typedef struct dailyschedule {
	char nameofday[16];
	char typeofday[16];
	int numperiods;
	period_s *periodlist;
} dailyschedule_s;

typedef struct schedule {
	dailyschedule_s dailyschedulelist[7];
} schedule_s;

static struct view_info {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *scroller;
	Evas_Object *box;
	Evas_Object *layout;
	Evas_Object *index;
	schedule_s *schedule;
	Evas_Object **listOfProgressBarsToUpdate;
	Evas_Object **listOfLabelsToUpdate;
	int wday;
	int hour;
	int minute;
	Ecore_Timer *timer;
} s_info = {
	.win = NULL,
	.conform = NULL,
	.scroller = NULL,
	.box = NULL,
	.layout = NULL,
	.index = NULL,
	.schedule = NULL,
    .listOfProgressBarsToUpdate = NULL,
    .listOfLabelsToUpdate = NULL,
	.wday = -1,
	.hour = -1,
	.minute = -1,
	.timer = NULL,
};

static void _win_delete_request_cb(void *user_data, Evas_Object *obj, void *event_info);
static void delete_all_labels_and_progressbars(Evas_Object *parent);
static void populateGUI(int day);
static void updateGUI(struct tm *timeinfo);
static char* _progress_format_cb(double val);
static void _progress_format_free(char *str);

void print_type(Evas_Object *obj) {
	const char *type = evas_object_type_get(obj);
	if (type) {
		dlog_print(DLOG_INFO, "MY_TAG", "Object type: %s\n", type);
	} else {
		dlog_print(DLOG_INFO, "MY_TAG", "Object type is unknown.\n");
	}
}

static schedule_s* populate_schedule()
{
	dailyschedule_s AnchorDaySchedule;
	dailyschedule_s OddDaySchedule;
	dailyschedule_s EvenDaySchedule;
	dailyschedule_s WeekendSchedule;
	static schedule_s schedule;
	time_t t;
//	time_t starttime;
//	time_t endtime;
	struct tm * timeinfo;

	// Get the current time
	t=time(NULL);
	timeinfo = localtime(&t);

	AnchorDaySchedule.numperiods = 10;
	AnchorDaySchedule.periodlist = (period_s *)malloc(AnchorDaySchedule.numperiods * sizeof(period_s));
	sprintf(AnchorDaySchedule.typeofday, "Anchor Day");
	AnchorDaySchedule.periodlist[0].majornum = 1;
	AnchorDaySchedule.periodlist[0].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[0].name, "Computers in Art");
	sprintf(AnchorDaySchedule.periodlist[0].room, "Room 116");
	AnchorDaySchedule.periodlist[0].start_hour = 7;
	AnchorDaySchedule.periodlist[0].start_min = 30;
	AnchorDaySchedule.periodlist[0].end_hour = 8;
	AnchorDaySchedule.periodlist[0].end_min = 14;
	AnchorDaySchedule.periodlist[0].duration_in_minutes = (AnchorDaySchedule.periodlist[0].end_hour-AnchorDaySchedule.periodlist[0].start_hour)*60+(AnchorDaySchedule.periodlist[0].end_min-AnchorDaySchedule.periodlist[0].start_min);
//	AnchorDaySchedule.periodlist[0].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[0].start_time.tm_hour = 7;
//	AnchorDaySchedule.periodlist[0].start_time.tm_min = 30;
//	AnchorDaySchedule.periodlist[0].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[0].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[0].end_time.tm_hour = 8;
//	AnchorDaySchedule.periodlist[0].end_time.tm_min = 14;
//	AnchorDaySchedule.periodlist[0].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[0].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[0].end_time));
//	AnchorDaySchedule.periodlist[0].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[1].majornum = 2;
	AnchorDaySchedule.periodlist[1].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[1].name, "Health & PE 7");
	sprintf(AnchorDaySchedule.periodlist[1].room, "Gym 1");
	AnchorDaySchedule.periodlist[1].start_hour = 8;
	AnchorDaySchedule.periodlist[1].start_min = 19;
	AnchorDaySchedule.periodlist[1].end_hour = 9;
	AnchorDaySchedule.periodlist[1].end_min = 1;
	AnchorDaySchedule.periodlist[1].duration_in_minutes = (AnchorDaySchedule.periodlist[1].end_hour-AnchorDaySchedule.periodlist[1].start_hour)*60+(AnchorDaySchedule.periodlist[1].end_min-AnchorDaySchedule.periodlist[1].start_min);
//	AnchorDaySchedule.periodlist[1].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[1].start_time.tm_hour = 8;
//	AnchorDaySchedule.periodlist[1].start_time.tm_min = 19;
//	AnchorDaySchedule.periodlist[1].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[1].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[1].end_time.tm_hour = 9;
//	AnchorDaySchedule.periodlist[1].end_time.tm_min = 1;
//	AnchorDaySchedule.periodlist[1].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[1].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[1].end_time));
//	AnchorDaySchedule.periodlist[1].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[2].majornum = 3;
	AnchorDaySchedule.periodlist[2].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[2].name, "Intermediate Orchestra");
	sprintf(AnchorDaySchedule.periodlist[2].room, "Room 163");
	AnchorDaySchedule.periodlist[2].start_hour = 9;
	AnchorDaySchedule.periodlist[2].start_min = 6;
	AnchorDaySchedule.periodlist[2].end_hour = 9;
	AnchorDaySchedule.periodlist[2].end_min = 48;
	AnchorDaySchedule.periodlist[2].duration_in_minutes = (AnchorDaySchedule.periodlist[2].end_hour-AnchorDaySchedule.periodlist[2].start_hour)*60+(AnchorDaySchedule.periodlist[2].end_min-AnchorDaySchedule.periodlist[2].start_min);
//	AnchorDaySchedule.periodlist[2].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[2].start_time.tm_hour = 9;
//	AnchorDaySchedule.periodlist[2].start_time.tm_min = 6;
//	AnchorDaySchedule.periodlist[2].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[2].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[2].end_time.tm_hour = 9;
//	AnchorDaySchedule.periodlist[2].end_time.tm_min = 48;
//	AnchorDaySchedule.periodlist[2].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[2].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[2].end_time));
//	AnchorDaySchedule.periodlist[2].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[3].majornum = 5;
	AnchorDaySchedule.periodlist[3].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[3].name, "Life Science HN");
	sprintf(AnchorDaySchedule.periodlist[3].room, "Room 146");
	AnchorDaySchedule.periodlist[3].start_hour = 9;
	AnchorDaySchedule.periodlist[3].start_min = 53;
	AnchorDaySchedule.periodlist[3].end_hour = 10;
	AnchorDaySchedule.periodlist[3].end_min = 35;
	AnchorDaySchedule.periodlist[3].duration_in_minutes = (AnchorDaySchedule.periodlist[3].end_hour-AnchorDaySchedule.periodlist[3].start_hour)*60+(AnchorDaySchedule.periodlist[3].end_min-AnchorDaySchedule.periodlist[3].start_min);
//	AnchorDaySchedule.periodlist[3].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[3].start_time.tm_hour = 9;
//	AnchorDaySchedule.periodlist[3].start_time.tm_min = 53;
//	AnchorDaySchedule.periodlist[3].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[3].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[3].end_time.tm_hour = 10;
//	AnchorDaySchedule.periodlist[3].end_time.tm_min = 35;
//	AnchorDaySchedule.periodlist[3].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[3].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[3].end_time));
//	AnchorDaySchedule.periodlist[3].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[4].majornum = 6;
	AnchorDaySchedule.periodlist[4].minornum = 1;
	sprintf(AnchorDaySchedule.periodlist[4].name, "SEL Lesson (Social Emotional Learning)");
	sprintf(AnchorDaySchedule.periodlist[4].room, "Room ???");
	AnchorDaySchedule.periodlist[4].start_hour = 10;
	AnchorDaySchedule.periodlist[4].start_min = 40;
	AnchorDaySchedule.periodlist[4].end_hour = 11;
	AnchorDaySchedule.periodlist[4].end_min = 10;
	AnchorDaySchedule.periodlist[4].duration_in_minutes = (AnchorDaySchedule.periodlist[4].end_hour-AnchorDaySchedule.periodlist[4].start_hour)*60+(AnchorDaySchedule.periodlist[4].end_min-AnchorDaySchedule.periodlist[4].start_min);
//	AnchorDaySchedule.periodlist[4].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[4].start_time.tm_hour = 10;
//	AnchorDaySchedule.periodlist[4].start_time.tm_min = 40;
//	AnchorDaySchedule.periodlist[4].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[4].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[4].end_time.tm_hour = 11;
//	AnchorDaySchedule.periodlist[4].end_time.tm_min = 10;
//	AnchorDaySchedule.periodlist[4].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[4].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[4].end_time));
//	AnchorDaySchedule.periodlist[4].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[5].majornum = 6;
	AnchorDaySchedule.periodlist[5].minornum = 2;
	sprintf(AnchorDaySchedule.periodlist[5].name, "B Lunch");
	sprintf(AnchorDaySchedule.periodlist[5].room, "Room ???");
	AnchorDaySchedule.periodlist[5].start_hour = 11;
	AnchorDaySchedule.periodlist[5].start_min = 10;
	AnchorDaySchedule.periodlist[5].end_hour = 11;
	AnchorDaySchedule.periodlist[5].end_min = 40;
	AnchorDaySchedule.periodlist[5].duration_in_minutes = (AnchorDaySchedule.periodlist[5].end_hour-AnchorDaySchedule.periodlist[5].start_hour)*60+(AnchorDaySchedule.periodlist[5].end_min-AnchorDaySchedule.periodlist[5].start_min);
//	AnchorDaySchedule.periodlist[5].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[5].start_time.tm_hour = 11;
//	AnchorDaySchedule.periodlist[5].start_time.tm_min = 10;
//	AnchorDaySchedule.periodlist[5].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[5].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[5].end_time.tm_hour = 11;
//	AnchorDaySchedule.periodlist[5].end_time.tm_min = 40;
//	AnchorDaySchedule.periodlist[5].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[5].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[5].end_time));
//	AnchorDaySchedule.periodlist[5].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[6].majornum = 6;
	AnchorDaySchedule.periodlist[6].minornum = 3;
	sprintf(AnchorDaySchedule.periodlist[6].name, "English 7 HN");
	sprintf(AnchorDaySchedule.periodlist[6].room, "Room 150");
	AnchorDaySchedule.periodlist[6].start_hour = 11;
	AnchorDaySchedule.periodlist[6].start_min = 40;
	AnchorDaySchedule.periodlist[6].end_hour = 12;
	AnchorDaySchedule.periodlist[6].end_min = 25;
	AnchorDaySchedule.periodlist[6].duration_in_minutes = (AnchorDaySchedule.periodlist[6].end_hour-AnchorDaySchedule.periodlist[6].start_hour)*60+(AnchorDaySchedule.periodlist[6].end_min-AnchorDaySchedule.periodlist[6].start_min);
//	AnchorDaySchedule.periodlist[6].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[6].start_time.tm_hour = 11;
//	AnchorDaySchedule.periodlist[6].start_time.tm_min = 40;
//	AnchorDaySchedule.periodlist[6].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[6].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[6].end_time.tm_hour = 12;
//	AnchorDaySchedule.periodlist[6].end_time.tm_min = 25;
//	AnchorDaySchedule.periodlist[6].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[6].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[6].end_time));
//	AnchorDaySchedule.periodlist[6].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[7].majornum = 6;
	AnchorDaySchedule.periodlist[7].minornum = 4;
	sprintf(AnchorDaySchedule.periodlist[7].name, "Recess");
	sprintf(AnchorDaySchedule.periodlist[7].room, "Room ???");
	AnchorDaySchedule.periodlist[7].start_hour = 12;
	AnchorDaySchedule.periodlist[7].start_min = 25;
	AnchorDaySchedule.periodlist[7].end_hour = 12;
	AnchorDaySchedule.periodlist[7].end_min = 40;
	AnchorDaySchedule.periodlist[7].duration_in_minutes = (AnchorDaySchedule.periodlist[7].end_hour-AnchorDaySchedule.periodlist[7].start_hour)*60+(AnchorDaySchedule.periodlist[7].end_min-AnchorDaySchedule.periodlist[7].start_min);
//	AnchorDaySchedule.periodlist[7].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[7].start_time.tm_hour = 12;
//	AnchorDaySchedule.periodlist[7].start_time.tm_min = 25;
//	AnchorDaySchedule.periodlist[7].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[7].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[7].end_time.tm_hour = 12;
//	AnchorDaySchedule.periodlist[7].end_time.tm_min = 40;
//	AnchorDaySchedule.periodlist[7].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[7].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[7].end_time));
//	AnchorDaySchedule.periodlist[7].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[8].majornum = 7;
	AnchorDaySchedule.periodlist[8].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[8].name, "US History 7 HN");
	sprintf(AnchorDaySchedule.periodlist[8].room, "Room 148");
	AnchorDaySchedule.periodlist[8].start_hour = 12;
	AnchorDaySchedule.periodlist[8].start_min = 45;
	AnchorDaySchedule.periodlist[8].end_hour = 13;
	AnchorDaySchedule.periodlist[8].end_min = 27;
	AnchorDaySchedule.periodlist[8].duration_in_minutes = (AnchorDaySchedule.periodlist[8].end_hour-AnchorDaySchedule.periodlist[8].start_hour)*60+(AnchorDaySchedule.periodlist[8].end_min-AnchorDaySchedule.periodlist[8].start_min);
//	AnchorDaySchedule.periodlist[8].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[8].start_time.tm_hour = 12;
//	AnchorDaySchedule.periodlist[8].start_time.tm_min = 45;
//	AnchorDaySchedule.periodlist[8].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[8].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[8].end_time.tm_hour = 13;
//	AnchorDaySchedule.periodlist[8].end_time.tm_min = 27;
//	AnchorDaySchedule.periodlist[8].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[8].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[8].end_time));
//	AnchorDaySchedule.periodlist[8].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	AnchorDaySchedule.periodlist[9].majornum = 8;
	AnchorDaySchedule.periodlist[9].minornum = 0;
	sprintf(AnchorDaySchedule.periodlist[9].name, "Mathematics 7 HN");
	sprintf(AnchorDaySchedule.periodlist[9].room, "Room 207");
	AnchorDaySchedule.periodlist[9].start_hour = 13;
	AnchorDaySchedule.periodlist[9].start_min = 32;
	AnchorDaySchedule.periodlist[9].end_hour = 14;
	AnchorDaySchedule.periodlist[9].end_min = 15;
	AnchorDaySchedule.periodlist[9].duration_in_minutes = (AnchorDaySchedule.periodlist[9].end_hour-AnchorDaySchedule.periodlist[9].start_hour)*60+(AnchorDaySchedule.periodlist[9].end_min-AnchorDaySchedule.periodlist[9].start_min);
//	AnchorDaySchedule.periodlist[9].start_time = *timeinfo;
//	AnchorDaySchedule.periodlist[9].start_time.tm_hour = 13;
//	AnchorDaySchedule.periodlist[9].start_time.tm_min = 32;
//	AnchorDaySchedule.periodlist[9].start_time.tm_sec = 0;
//	AnchorDaySchedule.periodlist[9].end_time = *timeinfo;
//	AnchorDaySchedule.periodlist[9].end_time.tm_hour = 14;
//	AnchorDaySchedule.periodlist[9].end_time.tm_min = 15;
//	AnchorDaySchedule.periodlist[9].end_time.tm_sec = 0;
//	starttime = mktime(&(AnchorDaySchedule.periodlist[9].start_time));
//	endtime = mktime(&(AnchorDaySchedule.periodlist[9].end_time));
//	AnchorDaySchedule.periodlist[9].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.numperiods = 6;

	OddDaySchedule.periodlist = (period_s *)malloc(OddDaySchedule.numperiods * sizeof(period_s));
	sprintf(OddDaySchedule.typeofday, "Odd/Blue Day");
	OddDaySchedule.periodlist[0].majornum = 1;
	OddDaySchedule.periodlist[0].minornum = 0;
	sprintf(OddDaySchedule.periodlist[0].name, "Computers in Art");
	sprintf(OddDaySchedule.periodlist[0].room, "Room 116");
	OddDaySchedule.periodlist[0].start_hour = 7;
	OddDaySchedule.periodlist[0].start_min = 30;
	OddDaySchedule.periodlist[0].end_hour = 8;
	OddDaySchedule.periodlist[0].end_min = 55;
	OddDaySchedule.periodlist[0].duration_in_minutes = (OddDaySchedule.periodlist[0].end_hour-OddDaySchedule.periodlist[0].start_hour)*60+(OddDaySchedule.periodlist[0].end_min-OddDaySchedule.periodlist[0].start_min);
//	OddDaySchedule.periodlist[0].start_time = *timeinfo;
//	OddDaySchedule.periodlist[0].start_time.tm_hour = 7;
//	OddDaySchedule.periodlist[0].start_time.tm_min = 30;
//	OddDaySchedule.periodlist[0].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[0].end_time = *timeinfo;
//	OddDaySchedule.periodlist[0].end_time.tm_hour = 8;
//	OddDaySchedule.periodlist[0].end_time.tm_min = 55;
//	OddDaySchedule.periodlist[0].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[0].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[0].end_time));
//	OddDaySchedule.periodlist[0].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.periodlist[1].majornum = 3;
	OddDaySchedule.periodlist[1].minornum = 0;
	sprintf(OddDaySchedule.periodlist[1].name, "Intermediate Orchestra");
	sprintf(OddDaySchedule.periodlist[1].room, "Room 163");
	OddDaySchedule.periodlist[1].start_hour = 9;
	OddDaySchedule.periodlist[1].start_min = 0;
	OddDaySchedule.periodlist[1].end_hour = 9;
	OddDaySchedule.periodlist[1].end_min = 45;
	OddDaySchedule.periodlist[1].duration_in_minutes = (OddDaySchedule.periodlist[1].end_hour-OddDaySchedule.periodlist[1].start_hour)*60+(OddDaySchedule.periodlist[1].end_min-OddDaySchedule.periodlist[1].start_min);
//	OddDaySchedule.periodlist[1].start_time = *timeinfo;
//	OddDaySchedule.periodlist[1].start_time.tm_hour = 9;
//	OddDaySchedule.periodlist[1].start_time.tm_min = 0;
//	OddDaySchedule.periodlist[1].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[1].end_time = *timeinfo;
//	OddDaySchedule.periodlist[1].end_time.tm_hour = 9;
//	OddDaySchedule.periodlist[1].end_time.tm_min = 45;
//	OddDaySchedule.periodlist[1].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[1].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[1].end_time));
//	OddDaySchedule.periodlist[1].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.periodlist[2].majornum = 4;
	OddDaySchedule.periodlist[2].minornum = 0;
	sprintf(OddDaySchedule.periodlist[2].name, "Learning Seminar and Recess");
	sprintf(OddDaySchedule.periodlist[2].room, "Room 207");
	OddDaySchedule.periodlist[2].start_hour = 9;
	OddDaySchedule.periodlist[2].start_min = 50;
	OddDaySchedule.periodlist[2].end_hour = 10;
	OddDaySchedule.periodlist[2].end_min = 40;
	OddDaySchedule.periodlist[2].duration_in_minutes = (OddDaySchedule.periodlist[2].end_hour-OddDaySchedule.periodlist[2].start_hour)*60+(OddDaySchedule.periodlist[2].end_min-OddDaySchedule.periodlist[2].start_min);
//	OddDaySchedule.periodlist[2].start_time = *timeinfo;
//	OddDaySchedule.periodlist[2].start_time.tm_hour = 9;
//	OddDaySchedule.periodlist[2].start_time.tm_min = 50;
//	OddDaySchedule.periodlist[2].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[2].end_time = *timeinfo;
//	OddDaySchedule.periodlist[2].end_time.tm_hour = 10;
//	OddDaySchedule.periodlist[2].end_time.tm_min = 40;
//	OddDaySchedule.periodlist[2].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[2].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[2].end_time));
//	OddDaySchedule.periodlist[2].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.periodlist[3].majornum = 5;
	OddDaySchedule.periodlist[3].minornum = 1;
	sprintf(OddDaySchedule.periodlist[3].name, "A Lunch");
	sprintf(OddDaySchedule.periodlist[3].room, "Room ???");
	OddDaySchedule.periodlist[3].start_hour = 10;
	OddDaySchedule.periodlist[3].start_min = 45;
	OddDaySchedule.periodlist[3].end_hour = 11;
	OddDaySchedule.periodlist[3].end_min = 15;
	OddDaySchedule.periodlist[3].duration_in_minutes = (OddDaySchedule.periodlist[3].end_hour-OddDaySchedule.periodlist[3].start_hour)*60+(OddDaySchedule.periodlist[3].end_min-OddDaySchedule.periodlist[3].start_min);
//	OddDaySchedule.periodlist[3].start_time = *timeinfo;
//	OddDaySchedule.periodlist[3].start_time.tm_hour = 10;
//	OddDaySchedule.periodlist[3].start_time.tm_min = 45;
//	OddDaySchedule.periodlist[3].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[3].end_time = *timeinfo;
//	OddDaySchedule.periodlist[3].end_time.tm_hour = 11;
//	OddDaySchedule.periodlist[3].end_time.tm_min = 15;
//	OddDaySchedule.periodlist[3].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[3].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[3].end_time));
//	OddDaySchedule.periodlist[3].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.periodlist[4].majornum = 5;
	OddDaySchedule.periodlist[4].minornum = 2;
	sprintf(OddDaySchedule.periodlist[4].name, "Life Science HN");
	sprintf(OddDaySchedule.periodlist[4].room, "Room 146");
	OddDaySchedule.periodlist[4].start_hour = 11;
	OddDaySchedule.periodlist[4].start_min = 15;
	OddDaySchedule.periodlist[4].end_hour = 12;
	OddDaySchedule.periodlist[4].end_min = 45;
	OddDaySchedule.periodlist[4].duration_in_minutes = (OddDaySchedule.periodlist[4].end_hour-OddDaySchedule.periodlist[4].start_hour)*60+(OddDaySchedule.periodlist[4].end_min-OddDaySchedule.periodlist[4].start_min);
//	OddDaySchedule.periodlist[4].start_time = *timeinfo;
//	OddDaySchedule.periodlist[4].start_time.tm_hour = 11;
//	OddDaySchedule.periodlist[4].start_time.tm_min = 15;
//	OddDaySchedule.periodlist[4].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[4].end_time = *timeinfo;
//	OddDaySchedule.periodlist[4].end_time.tm_hour = 12;
//	OddDaySchedule.periodlist[4].end_time.tm_min = 45;
//	OddDaySchedule.periodlist[4].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[4].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[4].end_time));
//	OddDaySchedule.periodlist[4].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	OddDaySchedule.periodlist[5].majornum = 7;
	OddDaySchedule.periodlist[5].minornum = 0;
	sprintf(OddDaySchedule.periodlist[5].name, "US History 7 HN");
	sprintf(OddDaySchedule.periodlist[5].room, "Room 148");
	OddDaySchedule.periodlist[5].start_hour = 12;
	OddDaySchedule.periodlist[5].start_min = 50;
	OddDaySchedule.periodlist[5].end_hour = 14;
	OddDaySchedule.periodlist[5].end_min = 15;
	OddDaySchedule.periodlist[5].duration_in_minutes = (OddDaySchedule.periodlist[5].end_hour-OddDaySchedule.periodlist[5].start_hour)*60+(OddDaySchedule.periodlist[5].end_min-OddDaySchedule.periodlist[5].start_min);
//	OddDaySchedule.periodlist[5].start_time = *timeinfo;
//	OddDaySchedule.periodlist[5].start_time.tm_hour = 12;
//	OddDaySchedule.periodlist[5].start_time.tm_min = 50;
//	OddDaySchedule.periodlist[5].start_time.tm_sec = 0;
//	OddDaySchedule.periodlist[5].end_time = *timeinfo;
//	OddDaySchedule.periodlist[5].end_time.tm_hour = 14;
//	OddDaySchedule.periodlist[5].end_time.tm_min = 15;
//	OddDaySchedule.periodlist[5].end_time.tm_sec = 0;
//	starttime = mktime(&(OddDaySchedule.periodlist[5].start_time));
//	endtime = mktime(&(OddDaySchedule.periodlist[5].end_time));
//	OddDaySchedule.periodlist[5].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.numperiods = 7;

	EvenDaySchedule.periodlist = (period_s *)malloc(EvenDaySchedule.numperiods * sizeof(period_s));
	sprintf(EvenDaySchedule.typeofday, "Even/Silver Day");
	EvenDaySchedule.periodlist[0].majornum = 2;
	EvenDaySchedule.periodlist[0].minornum = 0;
	sprintf(EvenDaySchedule.periodlist[0].name, "Health & PE 7");
	sprintf(EvenDaySchedule.periodlist[0].room, "Gym 1");
	EvenDaySchedule.periodlist[0].start_hour = 7;
	EvenDaySchedule.periodlist[0].start_min = 30;
	EvenDaySchedule.periodlist[0].end_hour = 8;
	EvenDaySchedule.periodlist[0].end_min = 55;
	EvenDaySchedule.periodlist[0].duration_in_minutes = (EvenDaySchedule.periodlist[0].end_hour-EvenDaySchedule.periodlist[0].start_hour)*60+(EvenDaySchedule.periodlist[0].end_min-EvenDaySchedule.periodlist[0].start_min);
//	EvenDaySchedule.periodlist[0].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[0].start_time.tm_hour = 7;
//	EvenDaySchedule.periodlist[0].start_time.tm_min = 30;
//	EvenDaySchedule.periodlist[0].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[0].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[0].end_time.tm_hour = 8;
//	EvenDaySchedule.periodlist[0].end_time.tm_min = 55;
//	EvenDaySchedule.periodlist[0].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[0].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[0].end_time));
//	EvenDaySchedule.periodlist[0].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[1].majornum = 3;
	EvenDaySchedule.periodlist[1].minornum = 0;
	sprintf(EvenDaySchedule.periodlist[1].name, "Intermediate Orchestra");
	sprintf(EvenDaySchedule.periodlist[1].room, "Room 163");
	EvenDaySchedule.periodlist[1].start_hour = 9;
	EvenDaySchedule.periodlist[1].start_min = 0;
	EvenDaySchedule.periodlist[1].end_hour = 9;
	EvenDaySchedule.periodlist[1].end_min = 45;
	EvenDaySchedule.periodlist[1].duration_in_minutes = (EvenDaySchedule.periodlist[1].end_hour-EvenDaySchedule.periodlist[1].start_hour)*60+(EvenDaySchedule.periodlist[1].end_min-EvenDaySchedule.periodlist[1].start_min);
//	EvenDaySchedule.periodlist[1].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[1].start_time.tm_hour = 9;
//	EvenDaySchedule.periodlist[1].start_time.tm_min = 0;
//	EvenDaySchedule.periodlist[1].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[1].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[1].end_time.tm_hour = 9;
//	EvenDaySchedule.periodlist[1].end_time.tm_min = 45;
//	EvenDaySchedule.periodlist[1].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[1].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[1].end_time));
//	EvenDaySchedule.periodlist[1].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[2].majornum = 4;
	EvenDaySchedule.periodlist[2].minornum = 0;
	sprintf(EvenDaySchedule.periodlist[2].name, "Learning Seminar and Recess");
	sprintf(EvenDaySchedule.periodlist[2].room, "Room 207");
	EvenDaySchedule.periodlist[2].start_hour = 9;
	EvenDaySchedule.periodlist[2].start_min = 50;
	EvenDaySchedule.periodlist[2].end_hour = 10;
	EvenDaySchedule.periodlist[2].end_min = 40;
	EvenDaySchedule.periodlist[2].duration_in_minutes = (EvenDaySchedule.periodlist[2].end_hour-EvenDaySchedule.periodlist[2].start_hour)*60+(EvenDaySchedule.periodlist[2].end_min-EvenDaySchedule.periodlist[2].start_min);
//	EvenDaySchedule.periodlist[2].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[2].start_time.tm_hour = 9;
//	EvenDaySchedule.periodlist[2].start_time.tm_min = 50;
//	EvenDaySchedule.periodlist[2].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[2].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[2].end_time.tm_hour = 10;
//	EvenDaySchedule.periodlist[2].end_time.tm_min = 40;
//	EvenDaySchedule.periodlist[2].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[2].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[2].end_time));
//	EvenDaySchedule.periodlist[2].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[3].majornum = 6;
	EvenDaySchedule.periodlist[3].minornum = 1;
	sprintf(EvenDaySchedule.periodlist[3].name, "English 7 HN");
	sprintf(EvenDaySchedule.periodlist[3].room, "Room 150");
	EvenDaySchedule.periodlist[3].start_hour = 10;
	EvenDaySchedule.periodlist[3].start_min = 45;
	EvenDaySchedule.periodlist[3].end_hour = 11;
	EvenDaySchedule.periodlist[3].end_min = 15;
	EvenDaySchedule.periodlist[3].duration_in_minutes = (EvenDaySchedule.periodlist[3].end_hour-EvenDaySchedule.periodlist[3].start_hour)*60+(EvenDaySchedule.periodlist[3].end_min-EvenDaySchedule.periodlist[3].start_min);
//	EvenDaySchedule.periodlist[3].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[3].start_time.tm_hour = 10;
//	EvenDaySchedule.periodlist[3].start_time.tm_min = 45;
//	EvenDaySchedule.periodlist[3].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[3].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[3].end_time.tm_hour = 11;
//	EvenDaySchedule.periodlist[3].end_time.tm_min = 15;
//	EvenDaySchedule.periodlist[3].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[3].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[3].end_time));
//	EvenDaySchedule.periodlist[3].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[4].majornum = 6;
	EvenDaySchedule.periodlist[4].minornum = 2;
	sprintf(EvenDaySchedule.periodlist[4].name, "B Lunch");
	sprintf(EvenDaySchedule.periodlist[4].room, "Room ???");
	EvenDaySchedule.periodlist[4].start_hour = 11;
	EvenDaySchedule.periodlist[4].start_min = 15;
	EvenDaySchedule.periodlist[4].end_hour = 11;
	EvenDaySchedule.periodlist[4].end_min = 45;
	EvenDaySchedule.periodlist[4].duration_in_minutes = (EvenDaySchedule.periodlist[4].end_hour-EvenDaySchedule.periodlist[4].start_hour)*60+(EvenDaySchedule.periodlist[4].end_min-EvenDaySchedule.periodlist[4].start_min);
//	EvenDaySchedule.periodlist[4].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[4].start_time.tm_hour = 11;
//	EvenDaySchedule.periodlist[4].start_time.tm_min = 15;
//	EvenDaySchedule.periodlist[4].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[4].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[4].end_time.tm_hour = 11;
//	EvenDaySchedule.periodlist[4].end_time.tm_min = 45;
//	EvenDaySchedule.periodlist[4].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[4].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[4].end_time));
//	EvenDaySchedule.periodlist[4].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[5].majornum = 6;
	EvenDaySchedule.periodlist[5].minornum = 3;
	sprintf(EvenDaySchedule.periodlist[5].name, "English 7 HN");
	sprintf(EvenDaySchedule.periodlist[5].room, "Room 150");
	EvenDaySchedule.periodlist[5].start_hour = 11;
	EvenDaySchedule.periodlist[5].start_min = 45;
	EvenDaySchedule.periodlist[5].end_hour = 12;
	EvenDaySchedule.periodlist[5].end_min = 45;
	EvenDaySchedule.periodlist[5].duration_in_minutes = (EvenDaySchedule.periodlist[5].end_hour-EvenDaySchedule.periodlist[5].start_hour)*60+(EvenDaySchedule.periodlist[5].end_min-EvenDaySchedule.periodlist[5].start_min);
//	EvenDaySchedule.periodlist[5].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[5].start_time.tm_hour = 11;
//	EvenDaySchedule.periodlist[5].start_time.tm_min = 45;
//	EvenDaySchedule.periodlist[5].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[5].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[5].end_time.tm_hour = 12;
//	EvenDaySchedule.periodlist[5].end_time.tm_min = 45;
//	EvenDaySchedule.periodlist[5].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[5].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[5].end_time));
//	EvenDaySchedule.periodlist[5].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	EvenDaySchedule.periodlist[6].majornum = 8;
	EvenDaySchedule.periodlist[6].minornum = 0;
	sprintf(EvenDaySchedule.periodlist[6].name, "Mathematics 7 HN");
	sprintf(EvenDaySchedule.periodlist[6].room, "Room 207");
	EvenDaySchedule.periodlist[6].start_hour = 12;
	EvenDaySchedule.periodlist[6].start_min = 50;
	EvenDaySchedule.periodlist[6].end_hour = 14;
	EvenDaySchedule.periodlist[6].end_min = 15;
	EvenDaySchedule.periodlist[6].duration_in_minutes = (EvenDaySchedule.periodlist[6].end_hour-EvenDaySchedule.periodlist[6].start_hour)*60+(EvenDaySchedule.periodlist[6].end_min-EvenDaySchedule.periodlist[6].start_min);
//	EvenDaySchedule.periodlist[6].start_time = *timeinfo;
//	EvenDaySchedule.periodlist[6].start_time.tm_hour = 12;
//	EvenDaySchedule.periodlist[6].start_time.tm_min = 50;
//	EvenDaySchedule.periodlist[6].start_time.tm_sec = 0;
//	EvenDaySchedule.periodlist[6].end_time = *timeinfo;
//	EvenDaySchedule.periodlist[6].end_time.tm_hour = 14;
//	EvenDaySchedule.periodlist[6].end_time.tm_min = 15;
//	EvenDaySchedule.periodlist[6].end_time.tm_sec = 0;
//	starttime = mktime(&(EvenDaySchedule.periodlist[6].start_time));
//	endtime = mktime(&(EvenDaySchedule.periodlist[6].end_time));
//	EvenDaySchedule.periodlist[6].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	WeekendSchedule.numperiods = 1;

	WeekendSchedule.periodlist = (period_s *)malloc(WeekendSchedule.numperiods * sizeof(period_s));
	sprintf(WeekendSchedule.typeofday, "Weekend");
	WeekendSchedule.periodlist[0].majornum = 0;
	WeekendSchedule.periodlist[0].minornum = 0;
	sprintf(WeekendSchedule.periodlist[0].name, "Weekend");
	sprintf(WeekendSchedule.periodlist[0].room, "Room ???");
	WeekendSchedule.periodlist[0].start_hour = 7;
	WeekendSchedule.periodlist[0].start_min = 30;
	WeekendSchedule.periodlist[0].end_hour = 14;
	WeekendSchedule.periodlist[0].end_min = 15;
	WeekendSchedule.periodlist[0].duration_in_minutes = (WeekendSchedule.periodlist[0].end_hour-WeekendSchedule.periodlist[0].start_hour)*60+(WeekendSchedule.periodlist[0].end_min-WeekendSchedule.periodlist[0].start_min);
//	WeekendSchedule.periodlist[0].start_time = *timeinfo;
//	WeekendSchedule.periodlist[0].start_time.tm_hour = 7;
//	WeekendSchedule.periodlist[0].start_time.tm_min = 30;
//	WeekendSchedule.periodlist[0].start_time.tm_sec = 0;
//	WeekendSchedule.periodlist[0].end_time = *timeinfo;
//	WeekendSchedule.periodlist[0].end_time.tm_hour = 14;
//	WeekendSchedule.periodlist[0].end_time.tm_min = 15;
//	WeekendSchedule.periodlist[0].end_time.tm_sec = 0;
//	starttime = mktime(&(WeekendSchedule.periodlist[0].start_time));
//	endtime = mktime(&(WeekendSchedule.periodlist[0].end_time));
//	WeekendSchedule.periodlist[0].duration_in_minutes = (int)(difftime(endtime, starttime)/60.0);

	schedule.dailyschedulelist[0] = AnchorDaySchedule;
	sprintf(schedule.dailyschedulelist[0].nameofday, "Monday");
	schedule.dailyschedulelist[1] = OddDaySchedule;
	sprintf(schedule.dailyschedulelist[1].nameofday, "Tuesday");
	schedule.dailyschedulelist[2] = EvenDaySchedule;
	sprintf(schedule.dailyschedulelist[2].nameofday, "Wednesday");
	schedule.dailyschedulelist[3] = OddDaySchedule;
	sprintf(schedule.dailyschedulelist[3].nameofday, "Thursday");
	schedule.dailyschedulelist[4] = EvenDaySchedule;
	sprintf(schedule.dailyschedulelist[4].nameofday, "Friday");
	schedule.dailyschedulelist[5] = WeekendSchedule;
	sprintf(schedule.dailyschedulelist[5].nameofday, "Saturday");
	schedule.dailyschedulelist[6] = WeekendSchedule;
	sprintf(schedule.dailyschedulelist[6].nameofday, "Sunday");

	return(&schedule);
}

static void populateGUI(int day)
{
	int i;
	Evas_Object *item = NULL;
	Evas_Object *padding = NULL;
	char tmp3[512];
	char tmp1[6];
	char tmp2[6];

//	dlog_print(DLOG_INFO, "MY_TAG", "Beginning of populateGUI()");
//	dlog_print(DLOG_INFO, "MY_TAG", "Current day is %d", day);
//	if (timer)
//	{
//		ecore_timer_del(timer);
//		timer = NULL;
//	}

//	time_t t = time(NULL);
//	struct tm *tm_info = localtime(&t);
//	s_info.wday = tm_info->tm_wday; // This gives 0=Sunday, 1=Monday ... etc.
//	s_info.wday = (s_info.wday+6)%7;
////	s_info.wday = 0;

	// Free the previously allocated memory
	if (s_info.listOfProgressBarsToUpdate != NULL)
	{
		free(s_info.listOfProgressBarsToUpdate);
		s_info.listOfProgressBarsToUpdate = NULL;
	}
	if (s_info.listOfLabelsToUpdate != NULL)
	{
		free(s_info.listOfLabelsToUpdate);
		s_info.listOfLabelsToUpdate = NULL;
	}
	s_info.listOfProgressBarsToUpdate = (Evas_Object **)malloc(s_info.schedule->dailyschedulelist[day].numperiods * sizeof(Evas_Object *));
	s_info.listOfLabelsToUpdate = (Evas_Object **)malloc(s_info.schedule->dailyschedulelist[day].numperiods * sizeof(Evas_Object *));

	delete_all_labels_and_progressbars(s_info.box);

	item = elm_label_add(s_info.layout);
	elm_label_line_wrap_set(item, ELM_WRAP_MIXED); // Enable line wrapping
	evas_object_size_hint_min_set(item, 200, 100); // Adjust the size as needed
	snprintf(tmp3, sizeof(tmp3), "<align=center>%s<br>%s</align>", s_info.schedule->dailyschedulelist[day].nameofday,s_info.schedule->dailyschedulelist[day].typeofday);
	elm_object_text_set(item, tmp3);
	evas_object_show(item);
	padding = evas_object_data_get(s_info.box, PADDING_END);
	elm_box_pack_before(s_info.box, item, padding);

	for (i = 0; i<s_info.schedule->dailyschedulelist[day].numperiods; i++)
	{
		item = elm_label_add(s_info.layout);
		elm_label_line_wrap_set(item, ELM_WRAP_MIXED); // Enable line wrapping
		evas_object_size_hint_min_set(item, 200, 100); // Adjust the size as needed
//		strftime(tmp1, sizeof(tmp1), "%H:%M", &(s_info.schedule->dailyschedulelist[day].periodlist[i].start_time));
//		strftime(tmp2, sizeof(tmp2), "%H:%M", &(s_info.schedule->dailyschedulelist[day].periodlist[i].end_time));
		snprintf(tmp1, sizeof(tmp1), "%02d:%02d", s_info.schedule->dailyschedulelist[day].periodlist[i].start_hour, s_info.schedule->dailyschedulelist[day].periodlist[i].start_min);
		snprintf(tmp2, sizeof(tmp1), "%02d:%02d", s_info.schedule->dailyschedulelist[day].periodlist[i].end_hour, s_info.schedule->dailyschedulelist[day].periodlist[i].end_min);
		if(s_info.schedule->dailyschedulelist[day].periodlist[i].minornum == 0)
		{
			snprintf(tmp3, sizeof(tmp3), "<color=#FFFFFF><align=center>Period %d<br>%s<br>%s<br>%s - %s<br>(%d min.)</align></color>", s_info.schedule->dailyschedulelist[day].periodlist[i].majornum, s_info.schedule->dailyschedulelist[day].periodlist[i].name, s_info.schedule->dailyschedulelist[day].periodlist[i].room, tmp1, tmp2, s_info.schedule->dailyschedulelist[day].periodlist[i].duration_in_minutes);
		}
		else
		{
			snprintf(tmp3, sizeof(tmp3), "<color=#FFFFFF><align=center>Period %d.%d<br>%s<br>%s<br>%s - %s<br>(%d min.)</align></color>", s_info.schedule->dailyschedulelist[day].periodlist[i].majornum, s_info.schedule->dailyschedulelist[day].periodlist[i].minornum, s_info.schedule->dailyschedulelist[day].periodlist[i].name, s_info.schedule->dailyschedulelist[day].periodlist[i].room, tmp1, tmp2, s_info.schedule->dailyschedulelist[day].periodlist[i].duration_in_minutes);
		}
		elm_object_text_set(item, tmp3);
		//elm_object_style_set(item, "slide_roll");
		//elm_label_slide_duration_set(item, 3);
		//elm_label_slide_mode_set(item, ELM_LABEL_SLIDE_MODE_ALWAYS);
		//evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(item);
		padding = evas_object_data_get(s_info.box, PADDING_END);
		elm_box_pack_before(s_info.box, item, padding);
//		Evas_Object* parent = evas_object_smart_parent_get(item);
//		print_type(item);
		s_info.listOfLabelsToUpdate[i] = item;

		item = elm_progressbar_add(s_info.layout);
		evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		//snprintf(tmp, sizeof(tmp), "Label %d", i);
		//elm_object_text_set(item,tmp);
		elm_progressbar_unit_format_function_set(item, _progress_format_cb,_progress_format_free);
		elm_progressbar_span_size_set(item, 200);
		evas_object_size_hint_align_set(item, EVAS_HINT_FILL*-1*0.5, 0.5);
		evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_progressbar_value_set(item, 0.5);
		evas_object_show(item);
		padding = evas_object_data_get(s_info.box, PADDING_END);
		elm_box_pack_before(s_info.box, item, padding);
		s_info.listOfProgressBarsToUpdate[i] = item;
//		print_type(item);

	}

//	timer_cb(NULL); //In case TIMER_INTERVAL is too long, call timer_cb first to update info immediately
//	timer = ecore_timer_add(TIMER_INTERVAL, timer_cb, NULL);

//	dlog_print(DLOG_INFO, "MY_TAG", "End of populateGUI()");
}

static void updateGUI(struct tm *timeinfo)
{
	int i;
	int diff_in_seconds;
	int duration;
	int mins_progressed;
	const char *current_text;
	const char *start;
	const char *end;
	char *color_start;
	double percent_complete;
	char updated_text[512]; // Adjust size as needed
	char color[7];
	char tmp3[40];
	size_t prefix_len;

//	dlog_print(DLOG_INFO, "MY_TAG", "Beginning of updateGUI()");
//	dlog_print(DLOG_INFO, "MY_TAG", "Current time: Day %d, Hour: %d, Min: %d, Sec:%d", s_info.wday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	for(i=0;i<s_info.schedule->dailyschedulelist[s_info.wday].numperiods;i++)
	{
		// Calculate the difference in seconds
//		diff_in_seconds = (int)difftime(mktime(timeinfo), mktime(&s_info.schedule->dailyschedulelist[s_info.wday].periodlist[i].start_time));
		diff_in_seconds = (timeinfo->tm_hour-s_info.schedule->dailyschedulelist[s_info.wday].periodlist[i].start_hour)*3600+(timeinfo->tm_min-s_info.schedule->dailyschedulelist[s_info.wday].periodlist[i].start_min)*60;
		percent_complete = ((double)diff_in_seconds)/(s_info.schedule->dailyschedulelist[s_info.wday].periodlist[i].duration_in_minutes*60);
		if (percent_complete<0.0)
		{
			strcpy(color,"FFFFFF");
		}
		else if (0.0<=percent_complete && percent_complete<1.0)
		{
			strcpy(color,"FFFF00");
		}
		else if (1.0<=percent_complete)
		{
			strcpy(color,"242424");
		}

		if(percent_complete<0.0)
		{
			percent_complete = 0.0;
		}
		else if (percent_complete>1.0)
		{
			percent_complete = 1.0;
		}
		elm_progressbar_value_set(s_info.listOfProgressBarsToUpdate[i], percent_complete);
		duration = s_info.schedule->dailyschedulelist[s_info.wday].periodlist[i].duration_in_minutes;
		mins_progressed = (int)(percent_complete*duration);
		snprintf(tmp3, sizeof(tmp3), "%d/%d min.", mins_progressed, duration);
		current_text = elm_object_text_get(s_info.listOfLabelsToUpdate[i]);
		start = strchr(current_text, '(');
		end = strchr(current_text, ')');
		// Step 3: Copy the text before the parentheses
		prefix_len = start - current_text + 1; // Include '('
		strncpy(updated_text, current_text, prefix_len);
		updated_text[prefix_len] = '\0';
		// Step 4: Append the new text inside the parentheses
		strncat(updated_text, tmp3, sizeof(updated_text) - strlen(updated_text) - 1);
		// Step 5: Append the text after the parentheses (including ')')
		strncat(updated_text, end, sizeof(updated_text) - strlen(updated_text) - 1);
		// Step 6: Find the location of the '#' character and replace the color code
		color_start = strstr(updated_text, "<color=#");
		color_start += 8; // Move the pointer to the start of the color code
		strncpy(color_start, color, 6); // Replace the 6 characters of the color code
//		dlog_print(DLOG_INFO, "MY_TAG", "%s", updated_text);
		elm_object_text_set(s_info.listOfLabelsToUpdate[i], updated_text);
	}

//	dlog_print(DLOG_INFO, "MY_TAG", "End of updateGUI()");
}

static Eina_Bool debug_clock_timer_cb(void *data)
{
	//dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is 1  : WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);
	time_t t = mktime(&debug_clock);
	//dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is 2  : WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);
	t = t+1;
	//dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is 3  : WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);
	struct tm *new_timeinfo = localtime(&t);
	//dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is 4  : WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);
	memcpy(&debug_clock, new_timeinfo, sizeof(struct tm));
	//dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is 5  : WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);

	// Return ECORE_CALLBACK_RENEW to keep the timer running
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool timer_cb(void *data)
{
//	dlog_print(DLOG_INFO, "MY_TAG", "Beginning of timer_cb()");
//	dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is: WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);

	if(s_info.timer)
	{
		ecore_timer_freeze(s_info.timer);
	}

    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);
    if(DEBUG_OVERRIDE_ENABLED)
	{
		timeinfo->tm_wday = debug_clock.tm_wday;
		timeinfo->tm_hour = debug_clock.tm_hour;
		timeinfo->tm_min = debug_clock.tm_min;
		timeinfo->tm_sec = debug_clock.tm_sec;
	}

    // tm_wday is in 0=Sunday, 1=Monday etc. format. Correct it to 0=Monday, 1=Tuesday etc. format
	int wday_corrected = (timeinfo->tm_wday+6)%7;

    /* check if hour, minute or day info changed. If it changed you may need to update info
     * on screen
     */
    if(wday_corrected != s_info.wday)
    {
    	populateGUI(wday_corrected);
    	s_info.wday = wday_corrected;
    }

    if(timeinfo->tm_hour != s_info.hour || timeinfo->tm_min != s_info.minute)
    {
    	updateGUI(timeinfo);
    	s_info.hour = timeinfo->tm_hour;
    	s_info.minute = timeinfo->tm_min;
    }

    if(s_info.timer)
    {
    	ecore_timer_thaw(s_info.timer);
    }

//    dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock time is: WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);
//    dlog_print(DLOG_INFO, "MY_TAG", "End of timer_cb()");

    // Return ECORE_CALLBACK_RENEW to keep the timer running
    return ECORE_CALLBACK_RENEW;
}

/* Format callback */
static char* _progress_format_cb(double val)
{
   static char buf[30];
   int files = (1-val)*14000;
   if (snprintf(buf, 30, "%i files left", files) > 0)
     return strdup(buf);
   return NULL;
}

static void _progress_format_free(char *str)
{
   free(str);
}

// Function to delete all child labels of a given parent
static void delete_all_labels_and_progressbars(Evas_Object *parent)
{
	Eina_List *children = evas_object_smart_members_get(parent);
	Eina_List *l;
	Evas_Object *child;

//	dlog_print(DLOG_INFO, "MY_TAG", "Beginning of delete_all_labels_and_progressbars()");
//	const char *type1 = evas_object_type_get(parent);
//	int count = eina_list_count(children);
//	dlog_print(DLOG_INFO, "MY_TAG", "type of parent is: %s\n", type1);
//	dlog_print(DLOG_INFO, "MY_TAG", "parent has %d children\n", count);
	EINA_LIST_FOREACH(children, l, child)
	{
		const char *type = evas_object_type_get(child);
//		dlog_print(DLOG_INFO, "MY_TAG", "type of child is: %s\n", type);
		if (strcmp(type, "elm_label") == 0 || strcmp(type, "elm_progressbar") == 0)
		{
			evas_object_del(child);
//			dlog_print(DLOG_INFO, "MY_TAG", "deleted child of type %s\n", type);
		}
		else if (strcmp(type, "Evas_Object_Box")==0)
		{
			// Recursively check the children of container objects
			delete_all_labels_and_progressbars(child);
		}
	}
	eina_list_free(children);

//	dlog_print(DLOG_INFO, "MY_TAG", "End of delete_all_labels_and_progressbars()");
}

/*
 * @brief: Hook to take necessary actions before main event loop starts
 * Initialize UI resources and application's data
 * If this function returns true, the main loop of application starts
 * If this function returns false, the application is terminated
 */
static bool app_create(void *user_data)
{
	Evas_Object *padding_start = NULL;
	Evas_Object *padding_end = NULL;

//	dlog_print(DLOG_INFO, "MY_TAG", "beginning of app_create()");

	/* Window */
	/*
	 * Create and initialize elm_win.
	 * elm_win is mandatory to manipulate window
	 */
	s_info.win = elm_win_util_standard_add("com.utkuturer.arensschedule", "com.utkuturer.arensschedule");
	elm_win_conformant_set(s_info.win, EINA_TRUE);
	elm_win_autodel_set(s_info.win, EINA_TRUE);

	/* Rotation setting */
	if (elm_win_wm_rotation_supported_get(s_info.win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(s_info.win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(s_info.win, "delete,request", _win_delete_request_cb, NULL);

	/*
	 * Conformant
	 * Create and initialize elm_conformant.
	 * elm_conformant is mandatory for base GUI to have proper size
	 * when indicator or virtual keypad is visible.
	 */
	s_info.conform = elm_conformant_add(s_info.win);
	evas_object_size_hint_weight_set(s_info.conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(s_info.win, s_info.conform);
	evas_object_show(s_info.conform);

	/* Show window after main view is set up */
	evas_object_show(s_info.win);

	/* Create layout by EDC(edje file) */
	s_info.layout = elm_layout_add(s_info.conform);
	evas_object_size_hint_weight_set(s_info.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(s_info.win, s_info.layout);
	elm_layout_theme_set(s_info.layout, "layout", "application", "content-back-next");
	evas_object_show(s_info.layout);
	elm_object_content_set(s_info.conform, s_info.layout);

	s_info.scroller = elm_scroller_add(s_info.layout);
	elm_scroller_content_min_limit(s_info.scroller, EINA_TRUE, EINA_FALSE);
	elm_scroller_bounce_set(s_info.scroller, EINA_TRUE, EINA_TRUE);
	/* Hide horizontal & vertical scroll bar */
	elm_scroller_policy_set(s_info.scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
	evas_object_size_hint_weight_set(s_info.scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(s_info.scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
	/* Can be scrolled vertically */
	elm_object_scroll_lock_x_set(s_info.scroller, EINA_TRUE);
	/* You can show effect when you reach to end of scroller */
	elm_object_style_set(s_info.scroller, "effect");
	//evas_object_smart_callback_add(s_info.scroller, "scroll", _scrolled_cb, NULL);
	/* pass part name as a second argument for the part which you want to fill */
	//elm_object_part_content_set(s_info.layout, GRP_SCROLLER, s_info.scroller);
	elm_object_part_content_set(s_info.layout, "elm.swallow.content", s_info.scroller);
	//elm_scroller_page_size_set(s_info.scroller, ICON_WIDTH*0, ICON_HEIGHT*0);
	evas_object_show(s_info.scroller);

	s_info.box = elm_box_add(s_info.scroller);
	evas_object_size_hint_weight_set(s_info.box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_fill_set(s_info.box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//elm_box_horizontal_set(s_info.box, EINA_TRUE);
	elm_box_homogeneous_set(s_info.box, EINA_FALSE);
	elm_box_padding_set(s_info.box, 0, 25);

	padding_start = evas_object_rectangle_add(evas_object_evas_get(s_info.scroller));
	evas_object_size_hint_min_set(padding_start, 1, 50);
	evas_object_color_set(padding_start, 255, 0, 0, 255);
	evas_object_show(padding_start);
	//evas_object_data_set(s_info.box, PADDING_START, padding_start);

	padding_end = evas_object_rectangle_add(evas_object_evas_get(s_info.scroller));
	evas_object_size_hint_min_set(padding_end, 1, 50);
	evas_object_color_set(padding_end, 0, 255, 0, 255);
	evas_object_show(padding_end);
	evas_object_data_set(s_info.box, PADDING_END, padding_end);

	elm_box_pack_start(s_info.box, padding_start);
	elm_box_pack_end(s_info.box, padding_end);

	elm_object_content_set(s_info.scroller, s_info.box);

	evas_object_show(s_info.box);

	evas_object_show(s_info.layout);

	s_info.schedule = populate_schedule();

//	dlog_print(DLOG_INFO, "MY_TAG", "end of app_create()");
//	populateGUI();

	return true;
}

/*
 * @brief: This callback function is called when another application
 * sends the launch request to the application
 */
static void app_control(app_control_h app_control, void *user_data)
{
	/* Handle the launch request. */
}

/*
 * @brief: This callback function is called each time
 * the application is completely obscured by another application
 * and becomes invisible to the user
 */
static void app_pause(void *user_data)
{
	/* Take necessary actions when application becomes invisible. */

	// Stop the timer when the app goes into the background
	ecore_timer_freeze(s_info.timer);

//	if (timer)
//	{
//		ecore_timer_del(timer);
//		timer = NULL;
//	}
}

/*
 * @brief: This callback function is called each time
 * the application becomes visible to the user
 */
static void app_resume(void *user_data)
{
//	dlog_print(DLOG_INFO, "MY_TAG", "Beginning of app_resume()");

	if(DEBUG_OVERRIDE_ENABLED)
	{
		if(debug_clock_timer == NULL)
		{
			time_t t = time(NULL);
			struct tm *timeinfo = localtime(&t);
			memcpy(&debug_clock, timeinfo, sizeof(struct tm));

			int override_day = DEBUG_OVERRIDE_DAY;
			int override_hour = DEBUG_OVERRIDE_HOUR;
			int override_min = DEBUG_OVERRIDE_MIN;
			int override_sec = DEBUG_OVERRIDE_SEC;
			int override_tm_wday = (override_day+1)%7;

			debug_clock.tm_mday = debug_clock.tm_mday + (override_tm_wday-timeinfo->tm_wday);
			debug_clock.tm_hour = override_hour;
			debug_clock.tm_min = override_min;
			debug_clock.tm_sec = override_sec;
			mktime(&debug_clock); //normalize time structure

//			dlog_print(DLOG_INFO, "MY_TAG", "Debug_Clock is initiated to: WDay %d, Hour %d, Min %d, Sec %d", debug_clock.tm_wday, debug_clock.tm_hour, debug_clock.tm_min, debug_clock.tm_sec);

			debug_clock_timer = ecore_timer_add(1.0, debug_clock_timer_cb, NULL);
		}
	}

	/* Take necessary actions when application becomes visible. */
	if(s_info.timer == NULL)
	{
		timer_cb(NULL); //In case TIMER_INTERVAL is too long, call timer_cb first to update info immediately
		s_info.timer = ecore_timer_add(TIMER_INTERVAL, timer_cb, NULL);
	}
	else
	{
		timer_cb(NULL); //In case TIMER_INTERVAL is too long, call timer_cb first to update info immediately
		ecore_timer_thaw(s_info.timer);
	}
//	timer_cb(NULL); //In case TIMER_INTERVAL is too long, call timer_cb first to update info immediately
//	timer = ecore_timer_add(TIMER_INTERVAL, timer_cb, NULL);

//	dlog_print(DLOG_INFO, "MY_TAG", "End of app_resume()");

//	//check if wday changed
//	time_t t = time(NULL);
//	struct tm *tm_info = localtime(&t);
//	int wday = tm_info->tm_wday; // This gives 0=Sunday, 1=Monday ... etc.
//	wday = (wday+6)%7;
//
//	if (wday == s_info.wday)
//	{
//		//no need to repopulate GUI, just start the timer
//		if (!timer)
//		{
//			timer_cb(NULL); //In case TIMER_INTERVAL is too long, call timer_cb first to update info immediately
//			timer = ecore_timer_add(TIMER_INTERVAL, timer_cb, NULL);
//		}
//	}
//	else
//	{
//		//day changed so schedule changed, repopulate GUI
////		dlog_print(DLOG_INFO, "MY_TAG", "Populating GUI due to app_resume");
//		populateGUI();
//	}
}

/*
 * @brief: This callback function is called once after the main loop of the application exits
 */
static void app_terminate(void *user_data)
{
	/* Release all resources. */
	if (s_info.win == NULL) {
		return;
	}

	evas_object_del(s_info.win);

	// Your app termination code here
	if (s_info.timer) {
		ecore_timer_del(s_info.timer);
		s_info.timer = NULL;
	}
}

/*
 * @brief: This function will be called when the language is changed
 */
static void ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;

	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);

	if (locale != NULL) {
		elm_language_set(locale);
		free(locale);
	}
	return;
}

/*
 * @brief: Main function of the application
 */
int main(int argc, char *argv[])
{
	int ret;

	ui_app_lifecycle_callback_s event_callback = {0, };
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	/*
	 * If you want to handling more events,
	 * Please check the application lifecycle guide
	 */
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, NULL);

	ret = ui_app_main(argc, argv, &event_callback, NULL);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}


/*
 * @brief: The function will be called when the window is deleted
 * @param[user_data] : The data passed to the function
 * @param[obj] : The object, normally window
 * @param[event_info] : Information regarding with the event
 */
static void _win_delete_request_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}
