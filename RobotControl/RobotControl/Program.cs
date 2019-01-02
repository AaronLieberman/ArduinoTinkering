using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotControl
{
    static class Program
    {
        [STAThread]
        public static void Main()
        {
            var app = new App();

            using (var servoManager = new ServoManager())
            {
                var window = new MainWindow
                {
                    DataContext = new RobotVM(servoManager)
                };
                window.Show();

                app.Run();
            }
        }
    }
}
