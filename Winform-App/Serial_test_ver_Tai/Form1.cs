using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing.Drawing2D;
using System.Threading;
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
namespace Serial_test_ver_Tai
{
    public partial class Form1 : Form
    {
        //Image img;
        private List<PointLatLng> _points;
        private List<PointLatLng> _pointsRec;
        //Graphics g;
        string dataReci;
        //double dataReci_new;
        string dataReci_new;
        string dataSend;
        double dataFloat1,dataFloat2;
        int length;
        //Pen p_red;
        // int count=0;
        GMapOverlay polyOverlay = new GMapOverlay("polygons");
        GMapOverlay markers = new GMapOverlay("markers");
        GMapOverlay routes_ = new GMapOverlay("routes");
        string[] arrListStr;



        public Form1()
        {
            GMapProviders.GoogleMap.ApiKey = @"AIzaSyDeyDCkJTCRGtsT0FbVPLLOr_8yrs7QbGQ";
            //p_red = new Pen(Color.Red, 1);
            InitializeComponent();
            getAvailablePorts();
            gMapControl1.ShowCenter = false; //xóa tâm đỏ trên map
            //g = label6.CreateGraphics();
            _points = new List<PointLatLng>();
            _pointsRec = new List<PointLatLng>();
           // img = Image.FromFile(@"C:\Users\trant\OneDrive\Desktop\edit-C#-ver2");
        }
        void getAvailablePorts()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.Text == "" || comboBox2.Text == "")
                {
                    //textBox2.Text = "Chọn Port Setting ";
                    MessageBox.Show(("Error"));
                    MessageBox.Show(("Please chose Port!"));
                }
                else
                {
                   
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                    serialPort1.Open();
                    progressBar1.Value = 100;
                    button1.Enabled = true;
                    button2.Enabled = true;
                    tboxSend.Enabled = true;
                    button3.Enabled = false;
                    button4.Enabled = true;
                    button5.Enabled = true;
                    button6.Enabled = true;
                    timer1.Enabled = true;
                    lbStatusCom.Text = "ON";

                }
            }
            catch (UnauthorizedAccessException)
            {

                //textBox2.Text = "Truy cập sai";
                lbStatusCom.Text = "OFF";


            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            progressBar1.Value = 0;
            button1.Enabled = false;
            button2.Enabled = false;
            button4.Enabled = false;
            button3.Enabled = true;
            button5.Enabled = false;
            button6.Enabled = false;
            tboxSend.Enabled = false;
            timer1.Enabled = false;
            lbStatusCom.Text = "OFF";

        }



        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                dataSend = tboxSend.Text;
                serialPort1.WriteLine(dataSend);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                textBox2.Text = serialPort1.ReadLine();
            }
            catch (TimeoutException)
            {
                textBox2.Text = "Timeout Exception";
            }
        }

        private void btstart_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                if (btstart.Text == "Start")
                {
                    serialPort1.WriteLine("r00000000 0000000000");
                    //serialPort1.WriteLine("r0000000");
                    btstart.Text = "Pause";
                    btstart.ForeColor = Color.Black;
                    btset.Enabled = false;

                }
                //else if (btstart.Text == "Resume")
                //{
                //    serialPort1.WriteLine("r000000000");
                //    btstart.Text = "Pause";
                //    btstart.ForeColor = Color.Black;
                //    btset.Enabled = false;
                //}
                //else
                //{
                //    serialPort1.WriteLine("s000000000");
                //    btstart.Text = "Resume";
                //    btstart.ForeColor = Color.Green;
                //    btset.Enabled = false;
                //}
            }
        }

        private void btstop_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.WriteLine("f00000000 0000000000");
                btstart.Text = "Start";
                btstart.ForeColor = Color.Green;
                btset.Enabled = true;
            }
        }

        private void bttransmit_Click(object sender, EventArgs e)
        {
            if (rbcircle.Checked)
            {
                if (serialPort1.IsOpen)
                {
                    serialPort1.WriteLine("c000000000");
                }
            }
            else if (rbpathl.Checked)
            {
                if (serialPort1.IsOpen)
                {
                    serialPort1.WriteLine("l000000000");
                }
            }
            btset.PerformClick();
        }

        private void btset_Click(object sender, EventArgs e)
        {
            if (btset.Text == "Setup")
            {
                btset.Text = "Cancel";
                groupBox3.Enabled = true;
            }
            else
            {
                btset.Text = "Setup";
                groupBox3.Enabled = false;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (tboxSend.Text != "")
            {
                tboxSend.Text = "";
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                //10.838784 106.671440
                serialPort1.WriteLine("000000000 0000000000");
            }
        }
       
        private void btLoad_Click(object sender, EventArgs e)
        {
            gMapControl1.Refresh();
            gMapControl1.Overlays.Clear();
            gMapControl1.DragButton = MouseButtons.Right;
          //  gMapControl1.MapProvider = GMapProviders.GoogleMap;
            //double lat = Convert.ToDouble(textLat.Text);
            //double lng = Convert.ToDouble(textLong.Text);
           // gMapControl1.Position = new PointLatLng(10.839890, 106.670098);
          //  gMapControl1.Position = new PointLatLng(dataFloat2, dataFloat1);
            gMapControl1.MinZoom = 1;
            gMapControl1.MaxZoom = 600;
            gMapControl1.Zoom = 10;

           
            //   PointLatLng point = new PointLatLng(10.839890, 106.670098);
            PointLatLng point = new PointLatLng(dataFloat2, dataFloat1);
            //GMapMarker marker = new GMarkerGoogle(point, GMarkerGoogleType.blue_dot);
            LoadMap(point);
            AddMarker(point);
            ////1 . Create a overlay
            //GMapOverlay markers = new GMapOverlay("markers");

            ////2 . Add all vailable markets to that Overlay
            //markers.Markers.Add(marker);

            ////3 . Cover map with Overlay
            //gMapControl1.Overlays.Add(markers);


        }


        
        private void gMapControl1_MouseClick(object sender, MouseEventArgs e)
        {
            //MessageBox.Show("Mouse Click");
            if (e.Button == MouseButtons.Left)
            {
                if (cbOffset.Checked)
                {

                    //-----------------------------------//
                    //TEST CODE KHÁC TIẾP

                    //double lat = _points[0].Lat;
                    //double lng = _points[0].Lng;
                    GMapOverlay routes = new GMapOverlay("routes");
                  
                    var point = gMapControl1.FromLocalToLatLng(e.X, e.Y);
                    //muốn vẽ liên tiếp 1 đoạn thì cmt dòng _points.Add(new PointLatLng(10.839890, 106.670098));
                    _points.Add(new PointLatLng(Math.Round(point.Lat, 6), Math.Round(point.Lng, 6)));
                    //_points.Add(new PointLatLng(10.839890, 106.670098));
                    _points.Add(new PointLatLng(dataFloat2, dataFloat1));
                    lbRecie.Items.Add(Convert.ToString(Math.Round(point.Lat, 6)) + " " + Convert.ToString(Math.Round(point.Lng, 6)));
                    tboxSend.Text = (Convert.ToString(Math.Round(point.Lat, 6)) + " " + Convert.ToString(Math.Round(point.Lng, 6)));

                    LoadMap(point);
                    AddMarker(point);

                    GMapRoute route = new GMapRoute(_points, "A walk in the park");
                    route.Stroke = new Pen(Color.Red, 3);
                    routes.Routes.Add(route);
                    gMapControl1.Overlays.Add(routes);   

                }
                // MessageBox.Show(e.X + " " + e.Y); for test

            }
        }

        private void LoadMap(PointLatLng point)
        {
            gMapControl1.Position = point;
        }
        
        private void AddMarker(PointLatLng pointToAdd, GMarkerGoogleType markerType = GMarkerGoogleType.blue_dot)
        {
            //var markers = new GMapOverlay("markers");
            var marker = new GMarkerGoogle(pointToAdd, markerType);
            markers.Markers.Add(marker);
            gMapControl1.Overlays.Add(markers);
        }

        private void btClearmap_Click(object sender, EventArgs e)
        {
            _points.Clear();
             gMapControl1.Refresh();
            lbRecie.Items.Clear();
            markers.Markers.Clear();
            gMapControl1.Overlays.Clear();
            routes_.Routes.Clear();
            // SerialPort.Flush();
            //dataFloat1 = 0;
            //dataFloat2 = 0;
            //serialPort1.DiscardInBuffer();
            if (gMapControl1.Overlays.Count > 0)
            {
                gMapControl1.Overlays.RemoveAt(0);
                gMapControl1.Refresh();
                //markers.Markers.Clear();
            }
        }
        //timer = 100 -> 0.1s
        
              //  dataFloat = Convert.ToDouble(dataReci_new);
               
        
        private void timer1_Tick(object sender, EventArgs e)
        {

            if (serialPort1.BytesToRead > 0)
            {
                
                //dataReci = serialPort1.ReadLine();
                if ((dataFloat2 > 0) && (dataFloat1 > 0))
                {
                    dataReci = serialPort1.ReadLine();
                    dataReci_new = dataReci;
                    //serialPort1.DiscardInBuffer();

                    GMapOverlay routes_ = new GMapOverlay("routes");
                    //  NewMethod();
                    getFloat();
                    _pointsRec.Add(new PointLatLng(dataFloat2, dataFloat1));
                    

                    GMapRoute route_ = new GMapRoute(_pointsRec, "A walk of robot");

                    listBox1.Items.Add(Convert.ToString(dataFloat2 + " " + dataFloat1));
                    listBox1.SelectedIndex = listBox1.Items.Count - 1;
                    listBox1.SelectedIndex = -1;
                    route_.Stroke = new Pen(Color.Blue, 3);
                    routes_.Routes.Add(route_);
                    gMapControl1.Overlays.Add(routes_);
                }
                
            }
        }
        
        

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Thread.Sleep(1000);
            dataReci = serialPort1.ReadExisting();
           // dataReci = serialPort1.ReadLine();
            this.Invoke(new EventHandler(ShowData));
            dataReci_new = dataReci;
            Thread.Sleep(1000);
            getFloat();
        }

        private void ShowData(object sender, EventArgs e)
        {
            label6.Text = dataReci;
            
        }

        private void button7_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.DiscardInBuffer();
                _pointsRec.Clear();
                //dataReci = "0";
                dataFloat1 = 0;
                dataFloat2 = 0;
                MessageBox.Show("Datas were cleared");
               // textBox1.Text = Convert.ToString(dataFloat2); //lat
              //  textBox3.Text = Convert.ToString(dataFloat1); //long  
                listBox1.Items.Clear();


            }
            catch
            {
                MessageBox.Show("Datas were'n cleared");
            } 
        }

        private void btView_Click(object sender, EventArgs e)
        {
            gMapControl1.MapProvider = GMapProviders.GoogleChinaSatelliteMap;
        }

        private void btNormal_Click(object sender, EventArgs e)
        {
            gMapControl1.MapProvider = GMapProviders.GoogleMap;
        }

        private void getFloat()
        {
            arrListStr = dataReci_new.Split(' ', Convert.ToChar(StringSplitOptions.RemoveEmptyEntries));
            //arrListStr = dataReci_new.Split(' ');
            length = arrListStr.Length;
            if (length == 2)
            {
                dataFloat1 = Math.Round(Convert.ToSingle(arrListStr[1]),6);

                dataFloat2 = Math.Round(Convert.ToSingle(arrListStr[0]),6);
                //textBox1.Text = Convert.ToString(dataFloat2); //lat
                //textBox3.Text = Convert.ToString(dataFloat1); //long  
            }
        }

    }
}
