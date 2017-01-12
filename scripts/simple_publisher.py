import rospy
import sys
from std_msgs.msg import Int32

class PublisherObject(object):
    """Instantiate this object to create a never ending publisher """

    def __init__(self, topic_name):
        """Constructor creates a publisher using the name given and starts an infinite publishing loop"""
        self.publisher = rospy.Publisher(topic_name, Int32, queue_size=1)

        rate = rospy.Rate(10) # 10hz
        i = 0

        while not rospy.is_shutdown():
            message = Int32() # create message
            message.data = i # add data to message
            i += 1

            self.publisher.publish(message) # publish the message
            rate.sleep()

if __name__ == '__main__':
    try:
        rospy.init_node("simple_publiser") # initialize the ros node
        publisher_object = PublisherObject("test_topic") # create and start the publishing instance
    except rospy.ROSInterruptException:
        sys.exit(1)
