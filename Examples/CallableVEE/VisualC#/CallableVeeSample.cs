using System;
using System.Windows.Forms;
using CallableVEE;

namespace CallableVeeSample
{
    public partial class CallableVeeSample : Form
    {
        private string _libFile = @"~installDir\examples\CallableVEE\lib.vee";
        private CallServer _server;
        private Library _library;
        private UserFunction _displayTestUF, _synthesizePulseUF;
        private double[,] _upperLimit = new double[11, 2];
        private double[,] _lowerLimit = new double[13, 2];

        public CallableVeeSample()
        {
            InitializeComponent();
        }

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new CallableVeeSample());
        }

        protected override void OnLoad(EventArgs e)
        {
            // Initialize upper and lower limit masks, only need to do this once.
            _upperLimit[0, 0] = 0;
            _upperLimit[0, 1] = 0.05;
            _upperLimit[1, 0] = 0.00000025;
            _upperLimit[1, 1] = 0.05;
            _upperLimit[2, 0] = 0.000000325;
            _upperLimit[2, 1] = 0.8;
            _upperLimit[3, 0] = 0.000000326;
            _upperLimit[3, 1] = 1.15;
            _upperLimit[4, 0] = 0.000000425;
            _upperLimit[4, 1] = 1.15;
            _upperLimit[5, 0] = 0.0000005;
            _upperLimit[5, 1] = 1.05;
            _upperLimit[6, 0] = 0.000000675;
            _upperLimit[6, 1] = 1.05;
            _upperLimit[7, 0] = 0.000000725;
            _upperLimit[7, 1] = -0.07;
            _upperLimit[8, 0] = 0.0000011;
            _upperLimit[8, 1] = 0.05;
            _upperLimit[9, 0] = 0.00000125;
            _upperLimit[9, 1] = 0.05;
            _upperLimit[10, 0] = 0.00000175;
            _upperLimit[10, 1] = 0.05;

            _lowerLimit[0, 0] = 0;
            _lowerLimit[0, 1] = -0.05;
            _lowerLimit[1, 0] = 0.00000035;
            _lowerLimit[1, 1] = -0.05;
            _lowerLimit[2, 0] = 0.000000351;
            _lowerLimit[2, 1] = 0.5;
            _lowerLimit[3, 0] = 0.0000004;
            _lowerLimit[3, 1] = 0.95;
            _lowerLimit[4, 0] = 0.0000005;
            _lowerLimit[4, 1] = 0.95;
            _lowerLimit[5, 0] = 0.0000006;
            _lowerLimit[5, 1] = 0.9;
            _lowerLimit[6, 0] = 0.00000065;
            _lowerLimit[6, 1] = 0.5;
            _lowerLimit[7, 0] = 0.000000651;
            _lowerLimit[7, 1] = -0.45;
            _lowerLimit[8, 0] = 0.0000008;
            _lowerLimit[8, 1] = -0.45;
            _lowerLimit[9, 0] = 0.000000925;
            _lowerLimit[9, 1] = -0.2;
            _lowerLimit[10, 0] = 0.0000011;
            _lowerLimit[10, 1] = -0.05;
            _lowerLimit[11, 0] = 0.00000125;
            _lowerLimit[11, 1] = -0.05;
            _lowerLimit[12, 0] = 0.00000175;
            _lowerLimit[12, 1] = -0.05;

            base.OnLoad(e);
        }

        private void executeTestButton_Click(object sender, EventArgs e)
        {
            // If server has never been created, then create it
            if (_server == null)
            {
                _server = new CallServerClass();

                // Have the server load the library of VEE UserFunctions
                _library = _server.Libraries.Load(_libFile);

                _synthesizePulseUF = _library.UserFunctions["Synthesize_Pulse"];
                _displayTestUF = _library.UserFunctions["Display_Test"];
            }

            // Get the Reference delay
            double delay = 0.0;
            double.TryParse(delayValyeTextBox.Text, out delay);
            delay *= 0.000001;

            // Check for proper range, if not then bail out of sub
            if ((delay < 0.00000025) || (delay > 0.00000045))
            {
                MessageBox.Show("Valid delay values are between .25 and .45 microseconds", "CallVEE Example", MessageBoxButtons.OK);
                return;
            }

            // Set the debug mode on the server
            _server.Debug = debugModeCheckBox.Checked;
            
            // Set up the input arguments to the "Synthesize_Pulse" UserFunction.
            // The UserFUnction has 1 input and 1 output
            object[] inputs = new object[1];
            object outputs = new object[1];
            inputs[0] = delay;

            // Call the Synthesize_Pulse UserFunction to create
            _synthesizePulseUF.Call(inputs, ref outputs);

            // The first parameter to "DisplaY_Test" is the synthesized pulse that was
            // returned from the userFunction call to "Synthesize_Pulse", we just made.
            VeeDataContainer pulseVDC = (VeeDataContainer)((Array)outputs).GetValue(0);


            // The second parameter to "DisplaY_Test" is an array of 2D Coords that
            // specify the upperLimit on our waveform mask.
            VeeDataContainer upperLimitVDC = _server.CreateVeeDataContainer();
            upperLimitVDC.Create2DCoord1DArray(_upperLimit);

            // The third parameter to "DisplaY_Test" is an array of 2D Coords that
            // specify the lowerLimit on our waveform mask.
            VeeDataContainer lowerLimitVDC = _server.CreateVeeDataContainer();
            lowerLimitVDC.Create2DCoord1DArray(_lowerLimit);

            // Set up the input arguments for the "Display_Test" UserFunction
            // The UserFUnction has 3 inputs and 1 output
            inputs = new object[3];
            outputs = new object[1];
            inputs[0] = pulseVDC;
            inputs[1] = upperLimitVDC;
            inputs[2] = lowerLimitVDC;

            // Call the "Display_Test" UserFunction
            _displayTestUF.Call(inputs, ref outputs);

            // Get the returned value. It is a 1D array of 2D coords
            VeeDataContainer errorsVDC = (VeeDataContainer)((Array)outputs).GetValue(0);

            // If the array is not empty then some points fell outside the mask
            if (errorsVDC.NumElements > 0)
            {
                MessageBox.Show("Test is failed!", "CallVEE Example", MessageBoxButtons.OK);
            }
        }
    }
}
