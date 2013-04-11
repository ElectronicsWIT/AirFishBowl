using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Microsoft.Kinect;
using System.Linq;

namespace CSKinectSkeletonApplication1
{
    public partial class MainWindow : Window
    {
        //Instantiate the Kinect runtime. Required to initialize the device.
        //IMPORTANT NOTE: You can pass the device ID here, in case more than one Kinect device is connected.
        ControlSender cs = new ControlSender();
        KinectSensor sensor = KinectSensor.KinectSensors[0];
        byte[] pixelData;
        Skeleton[] skeletons;

        public MainWindow()
        {
            InitializeComponent();

            //Runtime initialization is handled when the window is opened. When the window
            //is closed, the runtime MUST be unitialized.
            this.Loaded += new RoutedEventHandler(MainWindow_Loaded);
            this.Unloaded += new RoutedEventHandler(MainWindow_Unloaded);

            sensor.ColorStream.Enable();
            sensor.SkeletonStream.Enable();
            cs.start();
        }

        void runtime_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {
            bool receivedData = false;

            using (SkeletonFrame SFrame = e.OpenSkeletonFrame())
            {
                if (SFrame == null)
                {
                    // The image processing took too long. More than 2 frames behind.
                }
                else
                {
                    skeletons = new Skeleton[SFrame.SkeletonArrayLength];
                    SFrame.CopySkeletonDataTo(skeletons);
                    receivedData = true;
                }
            }

            if (receivedData)
            {

                Skeleton currentSkeleton = (from s in skeletons
                                            where s.TrackingState == SkeletonTrackingState.Tracked
                                            select s).FirstOrDefault();

                if (currentSkeleton != null)
                {
                    SetEllipsePosition(head, currentSkeleton.Joints[JointType.Head]);
                    SetEllipsePosition(leftHand, currentSkeleton.Joints[JointType.HandLeft]);
                    SetEllipsePosition(elbowleft, currentSkeleton.Joints[JointType.ElbowLeft]);
                    SetEllipsePosition(rightHand, currentSkeleton.Joints[JointType.HandRight]);
                    SetEllipsePosition(elbowright, currentSkeleton.Joints[JointType.ElbowRight]);
                    SetEllipsePosition(shoulderleft, currentSkeleton.Joints[JointType.ShoulderLeft]);
                    SetEllipsePosition(shoulderright, currentSkeleton.Joints[JointType.ShoulderRight]);
                    
                }
            }
        }


        //This method is used to position the ellipses on the canvas
        //according to correct movements of the tracked joints.

        //IMPORTANT NOTE: Code for vector scaling was imported from the Coding4Fun Kinect Toolkit
        //available here: http://c4fkinect.codeplex.com/
        //I only used this part to avoid adding an extra reference.
        private void SetEllipsePosition(Ellipse ellipse, Joint joint)
        {

            Int16 Y_Axis_Value;   //as line sugests
            Int16 X_Axis_Value;

            Microsoft.Kinect.SkeletonPoint vector = new Microsoft.Kinect.SkeletonPoint();
            vector.X = ScaleVector(780, joint.Position.X);
            vector.Y = ScaleVector(490, -joint.Position.Y);
            vector.Z = joint.Position.Z;

            Joint updatedJoint = new Joint();
            updatedJoint = joint;
            updatedJoint.TrackingState = JointTrackingState.Tracked;
            updatedJoint.Position = vector;

            Canvas.SetLeft(ellipse, updatedJoint.Position.X);
            Canvas.SetTop(ellipse, updatedJoint.Position.Y);

            Y_Axis_Value = Convert.ToInt16(vector.Y); //trying to print value to label
            Console.Write("Y Axis value :" + vector.Y);
            Y_Axis.Content = Y_Axis_Value;

            X_Axis_Value = Convert.ToInt16(vector.X);
            X_Axis.Content = X_Axis_Value;

     
            ///////// Selecting command values to send to WebPage 
            if (Y_Axis_Value < 240 && X_Axis_Value > 350 && X_Axis_Value < 450)           //Top Right Quadrant 
            {
                cs.setCommand("http://www.example.com");            //Forward 0     http://192.168.1.115/leds.cgi?led=0
                Direction.Content = "Forward";
            }
            else if (Y_Axis_Value < 240 && X_Axis_Value < 450)      //Top Left Quadrant 
            {
                cs.setCommand("http://www.example.com");            //Left 2       http://192.168.1.115/leds.cgi?led=2
                Direction.Content = "Left";
            }
            else if (Y_Axis_Value > 240)      //Bottom Right Quadrant 
            {
                cs.setCommand("http://www.example.com");            //Stop 1     http://192.168.1.115/leds.cgi?led=1
                Direction.Content = "Stop";
            }
            else if (Y_Axis_Value < 240 && X_Axis_Value > 350)      //Bottom Left Quadrant 
            {
                cs.setCommand("http://www.example.com");            //Right 3     http://192.168.1.115/leds.cgi?led=3
                Direction.Content = "Right";
            }
 
        }

        private float ScaleVector(int length, float position)
        {
            float value = (((((float)length) / 1f) / 2f) * position) + (length / 2);
            if (value > length)
            {
                return (float)length;
            }
            if (value < 0f)
            {
                return 0f;
            }
            return value;
        }

        void MainWindow_Unloaded(object sender, RoutedEventArgs e)
        {
            sensor.Stop();
        }

        void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            sensor.SkeletonFrameReady += runtime_SkeletonFrameReady;
            //sensor.ColorFrameReady += runtime_VideoFrameReady;        //This shows color video feed in window
            sensor.Start();
            sensor.ElevationAngle = 0;  //Default camera angle
        }

        void runtime_VideoFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            bool receivedData = false;

            using (ColorImageFrame CFrame = e.OpenColorImageFrame())
            {
                if (CFrame == null)
                {
                    // The image processing took too long. More than 2 frames behind.
                }
                else
                {
                    pixelData = new byte[CFrame.PixelDataLength];
                    CFrame.CopyPixelDataTo(pixelData);
                    receivedData = true;
                }
            }

            if (receivedData)
            {
                BitmapSource source = BitmapSource.Create(640, 480, 96, 96,        // was 640, 480,96,96
                        PixelFormats.Bgr32, null, pixelData, 640 * 4);          //Brg was 32

                videoImage.Source = source;
            }
        }

        /////////////////////////////////////////////
        //Start of Tilt Code
        /// /////////////////////////////////////////

        private void btnsetTilt_Click_1(object sender, RoutedEventArgs e)
        {
            Int32 TempStringValue;  //set up TempStringValue int to hold value

            if (txttilt.Text != string.Empty)
            {
                TempStringValue = Convert.ToInt32(txttilt.Text);    //make int equal to the value in thetxttilt.Text

                if (TempStringValue >= -27) //if its greater or equal to -27 enter loop else do nothing
                {
                    if (TempStringValue <= 27)  //if its less or equal to 27 enter loop else do nothing
                    {
                        sensor.ElevationAngle = Convert.ToInt32(txttilt.Text);  //set sensor angle to value in text box
                    }
                }
            }
        }


        /////////////////////////////////////////////
        //End of Tilt Code
        /////////////////////////////////////////////
    }

}
