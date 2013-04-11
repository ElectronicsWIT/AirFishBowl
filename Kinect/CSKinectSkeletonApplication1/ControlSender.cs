using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Diagnostics;
using System.Threading;

namespace CSKinectSkeletonApplication1
{
    class ControlSender
     {

         private volatile String currentCommand;
         private volatile bool running;
         private volatile Stopwatch sw = new Stopwatch();
         private volatile Thread threadMain;

         public void setCommand(String commandString)
         {
             currentCommand = commandString;
         }

         public void start()
         {
             Console.WriteLine("CS System : Starting");
             running = true;
             sw.Start();
             threadMain = new Thread(new ThreadStart(this.mainThread) );
             threadMain.Start();

         }

         public void stop()
         {
             Console.WriteLine("CS System : Stopping");
             running = false;
             threadMain.Join();
             sw.Reset();
         }

         private void mainThread()
         {
             while (running)
             {
                 if (sw.ElapsedMilliseconds >= 1000)
                 {
                     sw.Restart();
                     if (currentCommand != null)
                     {
                         Thread newThread = new Thread(new
ThreadStart(this.sendCommandThread));
                         newThread.Start();
                     }
                     else
                     {
                         Console.WriteLine("Command Empty! Use setCommand!");
                     }
                 }
             }
         }

         private void sendCommandThread()
         {
             HttpWebRequest request =
(HttpWebRequest)HttpWebRequest.Create(currentCommand);
             request.Proxy = null;
             HttpWebResponse response =
(HttpWebResponse)request.GetResponse();
             request.Abort();
             Console.WriteLine(response.StatusCode);
         }

     }
}
