/******************************************************************************
 * @section DESCRIPTION
 *
 * CESM driver time handling
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

 #include <vic_driver_cesm.h>

double dt_time_units = MISSING;
double numdate = MISSING;


/******************************************************************************
 * @brief    Initialize cesm time
 *****************************************************************************/
void
initialize_cesm_time(void)
{
    extern dmy_struct          dmy;
    extern global_param_struct global_param;

    // initialize time
    initialize_time();

    // Set dmy using global param start date/time
    dmy.year = global_param.startyear;
    dmy.month = global_param.startmonth;
    dmy.day = global_param.startday;
    dmy.dayseconds = global_param.startsec;

    // initialize module level globals
    dt_seconds_to_time_units(global_param.time_units, global_param.dt,
                             &dt_time_units);

    // initialize numdate
    numdate = date2num(global_param.time_origin_num, &dmy, 0.,
                       global_param.calendar, global_param.time_units);
}

/******************************************************************************
 * @brief    Advance one timestep
 *****************************************************************************/
void
advance_time(void)
{
    extern dmy_struct          dmy;
    extern global_param_struct global_param;

    numdate += dt_time_units;

    num2date(global_param.time_origin_num, numdate, 0., global_param.calendar,
             global_param.time_units, &dmy);

    print_dmy(&dmy);
}

/******************************************************************************
 * @brief    Raise an error if vclock and dmy don't match
 *****************************************************************************/
void
assert_time_insync(vic_clock  *vclock,
                   dmy_struct *dmy)
{
    if ((vclock->current_year != dmy->year) ||
        (vclock->current_month != dmy->month) ||
        (vclock->current_day != dmy->day) ||
        (vclock->current_dayseconds != dmy->dayseconds)) {
        print_vic_clock(vclock);
        print_dmy(dmy);
        log_err("Clocks out of sync");
    }
}
