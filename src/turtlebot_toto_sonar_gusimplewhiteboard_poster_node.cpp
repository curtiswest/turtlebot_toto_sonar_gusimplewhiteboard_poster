#include "ros/ros.h"
#include "sensor_msgs/Range.h"
#include "typeClassDefs/SENSORSSonarSensors.h"
#include "gugenericwhiteboardobject.h"
#include "guwhiteboardtypelist_generated.h"
#include <string>
#include <sstream>
#include <map>
#include <iostream>

#define TOTO_SONARS 12 //Use 12 for Toto
#define SONAR_CAP 255 //Max of 255 owing to whiteboard setup

using namespace guWhiteboard;

/**
 * From turtlebot_toto, we receive 12 sonar readings on 12 topics of type
 * sensors_msgs/Range.
 *
 * These are in theory 'hardware' limited to 255cm, but we still cap them to SONAR_CAP in case
 * noise pushes them over the limit, for example.
 */
class TotoSONARS {
public:
TotoSONARS (ros::NodeHandle n) {
        //Subscribe to sonar topics
        for(int i = 0; i < TOTO_SONARS; i++) {
                std::stringstream ss;
                ss << "sonar_" << i;
                subs[i] = n.subscribe<sensor_msgs::Range>(ss.str(), 10, boost::bind(&TotoSONARS::callback, this, _1, i));
                ROS_INFO("Subscribing to [%s]", ss.str().c_str());
        }

        //Initialise sonar readings to max
        for(int i = Sonar::Left0; i < Sonar::NUMBER_OF_READINGS; i++)
                sensors.set_sonar(uint8_t(SONAR_CAP), i);

        //Post to whiteboard
        SENSORSSonarSensors_t wb_handler; //gusimplewhiteboard
        wb_handler.post(sensors);
}

/**
 * The callback that posts the range messages to gusimplewhiteboard
 * @param sonar_in  [the sonar message]
 * @param sonar_num [the sonar from which we received this message]
 */
void callback (const sensor_msgs::Range::ConstPtr& sonar_in, int sonar_num) {
        uint8_t range = uint8_t((sonar_in->range*100.0) > SONAR_CAP ? SONAR_CAP : (sonar_in->range*100.0));
        fprintf(stdout, "s%-2i range: [%i]cm\n", sonar_num, range);

        switch(sonar_num) {
        case 0:
                sensors.set_sonar(range, Sonar::sZero);     break;
        case 1:
                sensors.set_sonar(range, Sonar::sOne);      break;
        case 2:
                sensors.set_sonar(range, Sonar::sTwo);      break;
        case 3:
                sensors.set_sonar(range, Sonar::sThree);    break;
        case 4:
                sensors.set_sonar(range, Sonar::sFour);     break;
        case 5:
                sensors.set_sonar(range, Sonar::sFive);     break;
        case 6:
                sensors.set_sonar(range, Sonar::sSix);      break;
        case 7:
                sensors.set_sonar(range, Sonar::sSeven);    break;
        case 8:
                sensors.set_sonar(range, Sonar::sEight);    break;
        case 9:
                sensors.set_sonar(range, Sonar::sNine);     break;
        case 10:
                sensors.set_sonar(range, Sonar::sTen);      break;
        case 11:
                sensors.set_sonar(range, Sonar::sEleven);   break;
        default:
                fprintf(stderr, "ERROR: trying to post  value (%i) to non-existent Toto sonar - source modification is necessary\n", sonar_num);
                break;
        }

        //Post updated values to whiteboard
        SENSORSSonarSensors_t wb_handler;
        wb_handler.post(sensors);
}

protected:
ros::Subscriber subs[TOTO_SONARS];
SENSORSSonarSensors sensors;                          // sensor values
double toto_sonars[TOTO_SONARS];
};

int main(int argc, char **argv) {
        ros::init(argc, argv, "sonar_wb_poster");
        ros::NodeHandle n;
        TotoSONARS subscribers(n);

        ros::spin();

        return 0;
}
