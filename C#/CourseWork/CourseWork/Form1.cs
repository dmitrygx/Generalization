using System;
using System.Drawing;
using System.Windows.Forms;

using RestSharp;
using System.Collections.Generic;

using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Text.RegularExpressions;
using System.Globalization;
using System.Text;

namespace CourseWork
{
    enum AppType
    {
        STANDALONE = 0,
        CLIENT = 1
    };
    public partial class Form1 : Form
    {
        AppType appType = AppType.CLIENT;
        string filename = "";
        string defaultFileName = "C:\\Users\\dgladkov\\Documents\\Education\\61LIN.DPF";
        Int32 CurrentCurve = -1;
        UInt32[] StepForCurves = new UInt32[10000];
        bool Initialize = false;
        bool[] OnlyResult = new bool[10000];
        bool[] PointsCanBeDrawed = new bool[10000];
        bool[] AdductionPointsCanBeDrawed = new bool[10000];
        bool[] SimplifactionPointsCanBeDrawed = new bool[10000];
        bool[] SmoothingPointsCanBeDrawed = new bool[10000];
        UInt32 DrawSelect = 0;
        public Form1()
        {
            for (UInt32 i = 0; i < 10000; ++i)
            {
                StepForCurves[i] = 0;
                OnlyResult[i] = false;
                PointsCanBeDrawed[i] = false;
                AdductionPointsCanBeDrawed[i] = false;
                SimplifactionPointsCanBeDrawed[i] = false;
                SmoothingPointsCanBeDrawed[i] = false;
            }
            InitializeComponent();
        }

        Curves curves;
        Double C = 0.5;
        private UInt32 Np = 500;
        private UInt32 Ns = 50;
        private Double f = 5;
        private UInt32 Ninit = 1000;
        private double M = 1;
        private int OpenMP = 0;
        private int IntelMKL = 0;
        bool taskBusy = false;
        RestClient client;

        public Task DoAsyncWork_RequestAllCurves(UInt32 ACount, UInt32[] countOfPoints)
        {
            Task task = new Task(() =>
            {
                taskBusy = true;
                Point[][] PointsInCurves = new Point[/*(UInt32)array.Count*/ACount][];
                for (UInt32 i = 0; i < curves.NumOfCurves; i++)
                {
                    if (countOfPoints[i] == 0)
                        continue;
                    StepForCurves[i] = 1;
                    //comboBox1.Items.Add("Curve " + (i + 1).ToString());

                    var requestCurvePoints = new RestRequest("/source_curve", Method.GET);

                    requestCurvePoints.AddParameter("curve_number", i);

                    IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                    if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                    {
                        System.Windows.Forms.MessageBox.Show("An error occured "
                            + queryCurvePointsResult.StatusCode.ToString()
                            + " during handling user's request to server.");
                        return;
                    }

                    JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                    UInt32 curveCountOfPoints = (UInt32)parsedCurveReq["count_points"];

                    JArray arrayOfPoints = (JArray)parsedCurveReq["points"];
                    System.Diagnostics.Debug.WriteLine(arrayOfPoints.ToString());
                    UInt32 k = 0;
                    PointsInCurves[i] = new Point[curveCountOfPoints];
                    foreach (JObject point in arrayOfPoints)
                    {

                        PointsInCurves[i][k].X = (UInt32)point["X"];
                        PointsInCurves[i][k].Y = (UInt32)point["Y"];
                        k++;
                    }
                }
                curves.InitializaCurves(PointsInCurves);



                return;
            });
            task.ContinueWith(t =>
            {
                taskBusy = false;
                for (UInt32 i = 0; i < curves.NumOfCurves; i++)
                {
                    if (curves.Items[i].CountPoints != 0)
                    {
                        comboBox1.Items.Add("Curve " + (i + 1).ToString());
                    }
                }
            }, TaskScheduler.FromCurrentSynchronizationContext());
            task.Start();
            return task;
        }

        public Task DoAsyncWork_RequestCurve(UInt32 CurveSerialNumber)
        {
            if (StepForCurves[CurveSerialNumber] == 1)
                return null;
            Task task = new Task(() =>
            {
                taskBusy = true;

                C = 0.5;
                Np = 500;
                Ns = 50;
                f = 5;
                Ninit = 1000;
                M = 1;
                if (textBox3.Text != "")
                {
                    C = System.Convert.ToDouble(textBox3.Text);
                    //System.Windows.Forms.MessageBox.Show("C = " + C.ToString());
                }
                if (textBox4.Text != "")
                {
                    Np = System.Convert.ToUInt32(textBox4.Text);
                    //System.Windows.Forms.MessageBox.Show("Np = " + Np.ToString());
                }
                if (textBox5.Text != "")
                {
                    Ns = System.Convert.ToUInt32(textBox5.Text);
                    //System.Windows.Forms.MessageBox.Show("Ns = " + Ns.ToString());
                }
                if (textBox6.Text != "")
                {
                    f = System.Convert.ToDouble(textBox6.Text);
                    //System.Windows.Forms.MessageBox.Show("f = " + f.ToString());
                }
                if (textBox7.Text != "")
                {
                    Ninit = System.Convert.ToUInt32(textBox7.Text);
                    //System.Windows.Forms.MessageBox.Show("Ninit = " + Ninit.ToString());
                }
                if (textBox11.Text != "")
                {
                    M = System.Convert.ToDouble(textBox11.Text);
                }

                var requestCurvePoints = new RestRequest("/source_curve", Method.GET);

                requestCurvePoints.AddParameter("curve_number", CurveSerialNumber);

                requestCurvePoints.AddParameter("params_C", C);
                requestCurvePoints.AddParameter("params_Np", Np);
                requestCurvePoints.AddParameter("params_Ns", Ns);
                requestCurvePoints.AddParameter("params_f", f);
                requestCurvePoints.AddParameter("params_Ninit", Ninit);
                requestCurvePoints.AddParameter("params_M", M);


                IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                                + queryCurvePointsResult.StatusCode.ToString()
                                + " during handling user's request to server.");
                    return;
                }

                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfPoints = (UInt32)parsedCurveReq["count_points"];

                JArray arrayOfPoints = (JArray)parsedCurveReq["points"];
                System.Diagnostics.Debug.WriteLine(arrayOfPoints.ToString());
                UInt32 k = 0;
                Point[] PointsInCurve = new Point[curveCountOfPoints];
                foreach (JObject point in arrayOfPoints)
                {
                    PointsInCurve[k].X = (UInt32)point["X"];
                    PointsInCurve[k].Y = (UInt32)point["Y"];
                    k++;
                }
                curves.InitializeCurve(PointsInCurve, CurveSerialNumber);
                return;
            });
            task.ContinueWith(t =>
            {
                taskBusy = false;
                StepForCurves[CurveSerialNumber] = 1;
                button2.Enabled = true;
                button9.Enabled = true;
                PointsCanBeDrawed[CurveSerialNumber] = true;
                comboBox2.Items.Add("1. Only initial points");
            }, TaskScheduler.FromCurrentSynchronizationContext());
            task.Start();
            return task;
        }

        public Task DoAsyncWork_Generalization(UInt32 CurveSerialNumber)
        {
            if (StepForCurves[CurveSerialNumber] > 1)
                return null;
            Task task = new Task(() =>
            {
                taskBusy = true;

                var request = new RestRequest("/generalization_curve", Method.GET);

                request.AddParameter("curve_number", CurveSerialNumber);

                IRestResponse queryCurvePointsResult = client.Execute(request);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                                + queryCurvePointsResult.StatusCode.ToString()
                                + " during handling user's request to server.");
                    return;
                }

                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfSegm = (UInt32)parsedCurveReq["count_segments"];
                UInt32[] CountOfAdductionPointsInSmoothSegment = new UInt32[curveCountOfSegm];
                UInt32 totalCountOfPoints = (UInt32)parsedCurveReq["total_count_points"];
                JArray arrayOfSegm = (JArray)parsedCurveReq["segments"];
                System.Diagnostics.Debug.WriteLine(arrayOfSegm.ToString());
                UInt32 k = 0;
                Point[,] AdductionPointsInSmoothSegment = new Point[curveCountOfSegm,
                                                                    curves.Items[CurrentCurve].AdductionCount];

                UInt32 segmIter = 0;

                foreach (JObject segm in arrayOfSegm)
                {
                    UInt32 count_of_points = (UInt32)segm["count_points"];
                    JArray arrayOfPoints = (JArray)segm["segment"];
                    CountOfAdductionPointsInSmoothSegment[segmIter] = count_of_points;

                    foreach (JObject point in arrayOfPoints)
                    {
                        AdductionPointsInSmoothSegment[segmIter, k].X = (UInt32)point["X"];
                        AdductionPointsInSmoothSegment[segmIter, k].Y = (UInt32)point["Y"];
                        k++;
                    }

                    segmIter++;
                    k = 0;
                }
                // means time for smoothing step
                Double time = (Double)parsedCurveReq["time"];
                Double overallTime = (Double)parsedCurveReq["overall_time"];
                Double sinuosityCoef_source = (Double)parsedCurveReq["sinuosity_coef_source"];
                Double sinuosityCoef_result = (Double)parsedCurveReq["sinuosity_coef_result"];

                curves.SetResultOfSmoothing(CurrentCurve, AdductionPointsInSmoothSegment,
                    curveCountOfSegm, CountOfAdductionPointsInSmoothSegment, totalCountOfPoints,
                    time);
                curves.SetResultsOfGeneralization(CurrentCurve, overallTime,
                    sinuosityCoef_source, sinuosityCoef_result);
                //curves.Items[CurrentCurve].Smoothing();
                //textBox8.Text = curves.Items[CurrentCurve].CountOfSegments.ToString();
                //textBox9.Text = curves.Items[CurrentCurve].ResultSegmentCount.ToString();
                textBox10.Text = curves.Items[CurrentCurve].TotalCountOfPointsAfterSimplification.ToString();
                SmoothingPointsCanBeDrawed[CurrentCurve] = true;
                return;
            });
            task.ContinueWith(t =>
            {
                taskBusy = false;
                OnlyResult[CurveSerialNumber] = true;
                StepForCurves[CurveSerialNumber] = 6;
                button2.Enabled = true;
                button3.Enabled = false;
                button4.Enabled = false;
                button6.Enabled = false;
                button9.Enabled = false;


                textBox12.Text = "";
                textBox13.Text = "";
                textBox14.Text = "";
                textBox15.Text = "";
                textBox16.Text = curves.Measurements[CurveSerialNumber].overall.ToString();
                textBox17.Text = "";
                textBox18.Text = "";

                comboBox2.Items.Add("6. Only smoothing points");
                comboBox2.Items.Add("7. Intitial points and smoothing points");
            }, TaskScheduler.FromCurrentSynchronizationContext());
            task.Start();
            return task;
        }

        static string EncodeNonAsciiCharacters(string value)
        {
            StringBuilder sb = new StringBuilder();
            foreach (char c in value)
            {
                if (c > 127)
                {
                    // This character is too big for ASCII
                    string encodedValue = "\\u" + ((int)c).ToString("x4");
                    sb.Append(encodedValue);
                }
                else
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        static string DecodeEncodedNonAsciiCharacters(string value)
        {
            return Regex.Replace(
                value,
                @"\\u(?<Value>[a-zA-Z0-9]{4})",
                m => {
                    return ((char)int.Parse(m.Groups["Value"].Value, NumberStyles.HexNumber)).ToString();
                });
        }

        /**
         * This is 1 step
         */
        private void button1_Click(object sender, EventArgs e)
        {
            if (!Initialize)
            {
                if (appType == AppType.CLIENT)
                {
                    client = new RestClient("http://localhost/generalization_server");

                    OpenMP = (checkBox1.Checked ? 1 : 0);
                    IntelMKL = (checkBox2.Checked ? 1 : 0);

                    var request = new RestRequest("/initialize", Method.GET);
                    string select;
                    if (comboBox3.SelectedItem != null)
                    {
                        select = comboBox3.SelectedItem.ToString();
                        if (select == null)
                        {
                            select = "File";
                        }
                    }
                    else
                    {
                        select = "File";
                    }

                    C = 0.5;
                    Np = 500;
                    Ns = 50;
                    f = 5;
                    Ninit = 1000;
                    M = 1;
                    if (textBox3.Text != "")
                    {
                        textBox3.Text.Replace('.', ',');
                        C = System.Convert.ToDouble(textBox3.Text);
                        //System.Windows.Forms.MessageBox.Show("C = " + C.ToString());
                    }
                    if (textBox4.Text != "")
                    {
                        Np = System.Convert.ToUInt32(textBox4.Text);
                        //System.Windows.Forms.MessageBox.Show("Np = " + Np.ToString());
                    }
                    if (textBox5.Text != "")
                    {
                        Ns = System.Convert.ToUInt32(textBox5.Text);
                        //System.Windows.Forms.MessageBox.Show("Ns = " + Ns.ToString());
                    }
                    if (textBox6.Text != "")
                    {
                        f = System.Convert.ToDouble(textBox6.Text);
                        //System.Windows.Forms.MessageBox.Show("f = " + f.ToString());
                    }
                    if (textBox7.Text != "")
                    {
                        Ninit = System.Convert.ToUInt32(textBox7.Text);
                        //System.Windows.Forms.MessageBox.Show("Ninit = " + Ninit.ToString());
                    }
                    if (textBox11.Text != "")
                    {
                        M = System.Convert.ToDouble(textBox11.Text);
                    }

                    request.AddParameter("params_C", C);
                    request.AddParameter("params_Np", Np);
                    request.AddParameter("params_Ns", Ns);
                    request.AddParameter("params_f", f);
                    request.AddParameter("params_Ninit", Ninit);
                    request.AddParameter("params_M", M);

                    request.AddParameter("type", select);
                    request.AddParameter("path", filename != "" ? filename : defaultFileName);

                    request.AddParameter("params_OpenMP", OpenMP);
                    request.AddParameter("params_IntelMKL", IntelMKL);

                    IRestResponse queryResult = client.Execute(request);
                    if (queryResult.StatusCode != System.Net.HttpStatusCode.OK)
                    {
                        System.Windows.Forms.MessageBox.Show("An error occured "
                            + queryResult.StatusCode.ToString()
                            + " during handling user's request to serer.");

                        return;
                    }

                    JObject parsedReq = JObject.Parse(queryResult.Content);

                    JArray array = (JArray)parsedReq["curves"];

                    System.Diagnostics.Debug.WriteLine(array.ToString());

                    UInt32[] countOfPoints = new UInt32[array.Count];
                    DBInfo[] dbInfo = new DBInfo[array.Count];
                    UInt32 count = 0;
                    foreach (JObject arrayObj in array)
                    {
                        UInt32 parsedCount = (UInt32)arrayObj["count_points"];
                        countOfPoints[count] = parsedCount;


                        String curveDBCode = (String)arrayObj["code"].ToString();
                        long curveDBNumber = (long)arrayObj["number"];

                        dbInfo[count].Code = curveDBCode;
                        dbInfo[count].Number = curveDBNumber;
                        count++;
                        //System.Windows.Forms.MessageBox.Show(parsedCount.ToString());
                    }
                    curves = new Curves((UInt32)array.Count, countOfPoints, C, Np, Ns, f, Ninit);
                    //Point[][] PointsInCurves = new Point[(UInt32)array.Count][];

                    StepForCurves = new UInt32[curves.NumOfCurves];
                    bool[] PointsCanBeDrawed = new bool[curves.NumOfCurves];
                    bool[] AdductionPointsCanBeDrawed = new bool[curves.NumOfCurves];
                    bool[] SimplifactionPointsCanBeDrawed = new bool[curves.NumOfCurves];
                    bool[] SmoothingPointsCanBeDrawed = new bool[curves.NumOfCurves];

                    //int result = await WaitAsyncWork((UInt32)array.Count);
                    Point[][] PointsInCurves = new Point[(UInt32)array.Count][];

                    for (UInt32 i = 0; i < curves.NumOfCurves; i++)
                    {
                        if (curves.Items[i].CountPoints != 0)
                        {
                            comboBox1.Items.Add("Curve " + (i + 1).ToString() + " (" +
                                dbInfo[i].Code + ":" + dbInfo[i].Number + ")");
                        }
                    }
                }
            }
        }

        /**
         * This is 2 step
         */
        private void button2_Click(object sender, EventArgs e)
        {
            if (((-1 != CurrentCurve) && (1 == StepForCurves[CurrentCurve])) ||
                (6 == StepForCurves[CurrentCurve]))
            {
                //curves.Items[CurrentCurve].Adduction();

                var requestCurvePoints = new RestRequest("/adduction_curve", Method.GET);

                requestCurvePoints.AddParameter("curve_number", CurrentCurve);
                requestCurvePoints.AddParameter("result",
                    System.Convert.ToInt32(OnlyResult[CurrentCurve]));

                IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                        + queryCurvePointsResult.StatusCode.ToString()
                        + " during handling user's request to server.");
                    return;
                }

                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfPoints = (UInt32)parsedCurveReq["count_points"];

                JArray arrayOfPoints = (JArray)parsedCurveReq["points"];
                System.Diagnostics.Debug.WriteLine(arrayOfPoints.ToString());
                UInt32 k = 0;
                Point[] AdductionPoints = new Point[curveCountOfPoints];
                foreach (JObject point in arrayOfPoints)
                {

                    AdductionPoints[k].X = (UInt32)point["X"];
                    AdductionPoints[k].Y = (UInt32)point["Y"];
                    k++;
                }
                Double spentTime = (Double)parsedCurveReq["time"];
                curves.SetResultOfAdduction(CurrentCurve, AdductionPoints, curveCountOfPoints, spentTime);

                button2.Enabled = false;
                button3.Enabled = true;
                button5.Enabled = false;
                button6.Enabled = false;
                button9.Enabled = false;

                textBox12.Text = curves.Measurements[CurrentCurve].adduction.ToString();
                if (6 != StepForCurves[CurrentCurve]) {
                    StepForCurves[CurrentCurve] = 2;
                }
                textBox2.Text = curves.Items[CurrentCurve].AdductionCount.ToString();
                AdductionPointsCanBeDrawed[CurrentCurve] = true;
                comboBox2.Items.Add("2. Only adduction points");
                comboBox2.Items.Add("3. Intitial points and adduction points");
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("ERROR: Illegal step of Algorithm. Expected step is 1," +
                                                     ((CurrentCurve != -1) ? " but current step is " + StepForCurves[CurrentCurve].ToString() : 
                                                     " but current curve is " + CurrentCurve.ToString()));
                if ((Initialize) && (CurrentCurve == -1))
                {
                    System.Windows.Forms.MessageBox.Show("Please, select curve from the list");
                }
            }
        }

        /**
         * This is 3 step
         */
        private void button3_Click(object sender, EventArgs e)
        {
            if (((-1 != CurrentCurve) && (2 == StepForCurves[CurrentCurve]))  ||
                (6 == StepForCurves[CurrentCurve]))
            {
                //curves.Items[CurrentCurve].Segmentation();

                var requestCurvePoints = new RestRequest("/segmentation_curve", Method.GET);

                requestCurvePoints.AddParameter("curve_number", CurrentCurve);
                requestCurvePoints.AddParameter("result",
                    System.Convert.ToInt32(OnlyResult[CurrentCurve]));

                IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                        + queryCurvePointsResult.StatusCode.ToString()
                        + " during handling user's request to server.");
                    return;
                }

                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfInitialSegm = (UInt32)parsedCurveReq["count_init_segments"];
                UInt32 curveCountOfSegm = (UInt32)parsedCurveReq["count_segments"];
                UInt32[] CountOfAdductionPointsInSegment = new UInt32[curveCountOfSegm];

                JArray arrayOfSegm = (JArray)parsedCurveReq["segments"];
                System.Diagnostics.Debug.WriteLine(arrayOfSegm.ToString());
                UInt32 k = 0;
                Point[,] AdductionPointsInSegment = new Point[curveCountOfSegm,
                                                              curves.Items[CurrentCurve].AdductionCount];

                UInt32 segmIter = 0;

                foreach (JObject segm in arrayOfSegm)
                {
                    UInt32 count_of_points = (UInt32)segm["count_points"];
                    JArray arrayOfPoints = (JArray)segm["segment"];
                    CountOfAdductionPointsInSegment[segmIter] = count_of_points;

                    foreach (JObject point in arrayOfPoints)
                    {
                        AdductionPointsInSegment[segmIter, k].X = (UInt32)point["X"];
                        AdductionPointsInSegment[segmIter, k].Y = (UInt32)point["Y"];
                        k++;
                    }

                    segmIter++;
                    k = 0;
                }
                Double spentTime = (Double)parsedCurveReq["time"];
                curves.SetResultOfSegmentation(CurrentCurve, AdductionPointsInSegment,
                    curveCountOfSegm, curveCountOfInitialSegm, CountOfAdductionPointsInSegment,
                    spentTime);

                textBox8.Text = curves.Items[CurrentCurve].CountOfSegments.ToString();
                textBox9.Text = curves.Items[CurrentCurve].ResultSegmentCount.ToString();

                button2.Enabled = false;
                button3.Enabled = false;
                button5.Enabled = true;
                button6.Enabled = false;
                button9.Enabled = false;
                textBox13.Text = curves.Measurements[CurrentCurve].segmentation.ToString();
                if (6 != StepForCurves[CurrentCurve])
                {
                    StepForCurves[CurrentCurve] = 3;
                }
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("ERROR: Illegal step of Algorithm. Expected step is 2," +
                                                     ((CurrentCurve != -1) ? " but current step is " + StepForCurves[CurrentCurve].ToString() :
                                                     " but current curve is " + CurrentCurve.ToString()));
                if ((Initialize) && (CurrentCurve == -1))
                {
                    System.Windows.Forms.MessageBox.Show("Please, select curve from the list");
                }
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            GC.Collect();
            GC.WaitForPendingFinalizers();
            Bitmap CurveOut = new Bitmap(10000, 10000);
            if (pictureBox1.Image != null)
            {
                pictureBox1.Image.Dispose();
                pictureBox1.Image = null;
            }
            if (0 == DrawSelect)
            {
                float saveOldX = 0;
                float saveOldY = 0;
                Pen pen = new Pen(Color.Red, 1);
                Pen pen1 = new Pen(Color.Blue, 1);

                CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[0].X, (int)curves.Items[CurrentCurve].Point[0].Y, Color.Red);
                Graphics g = Graphics.FromImage(CurveOut);
                g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[0].X, (float)curves.Items[CurrentCurve].Point[0].Y, 2, 2);
                saveOldX = (float)curves.Items[CurrentCurve].Point[0].X;
                saveOldY = (float)curves.Items[CurrentCurve].Point[0].Y;
                for (uint i = 1; i < curves.Items[CurrentCurve].CountPoints; ++i)
                {
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[i].X, (int)curves.Items[CurrentCurve].Point[i].Y, Color.Red);
                    PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y) };
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y, 2, 2);
                    g.DrawLines(pen, points);
                    saveOldX = (float)curves.Items[CurrentCurve].Point[i].X;
                    saveOldY = (float)curves.Items[CurrentCurve].Point[i].Y;
                }
            }
            else if (1 == DrawSelect)
            {
                float saveOldX = 0;
                float saveOldY = 0;
                Pen pen = new Pen(Color.Red, 1);
                Pen pen1 = new Pen(Color.Blue, 1);
                //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                CurveOut.SetPixel((int)curves.Items[CurrentCurve].AdductionPoints[0].X, (int)curves.Items[CurrentCurve].AdductionPoints[0].Y, Color.Red);
                Graphics g = Graphics.FromImage(CurveOut);
                g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].AdductionPoints[0].X, (float)curves.Items[CurrentCurve].AdductionPoints[0].Y, 2, 2);
                saveOldX = (float)curves.Items[CurrentCurve].AdductionPoints[0].X;
                saveOldY = (float)curves.Items[CurrentCurve].AdductionPoints[0].Y;
                for (uint i = 1; i < curves.Items[CurrentCurve].AdductionCount; ++i)
                {
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].AdductionPoints[i].X, (int)curves.Items[CurrentCurve].AdductionPoints[i].Y, Color.Red);
                    PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].AdductionPoints[i].X, (float)curves.Items[CurrentCurve].AdductionPoints[i].Y) };
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].AdductionPoints[i].X, (float)curves.Items[CurrentCurve].AdductionPoints[i].Y, 2, 2);
                    g.DrawLines(pen, points);
                    saveOldX = (float)curves.Items[CurrentCurve].AdductionPoints[i].X;
                    saveOldY = (float)curves.Items[CurrentCurve].AdductionPoints[i].Y;
                }
            }
            else if (2 == DrawSelect)
            {
                float saveOldX = 0;
                float saveOldY = 0;
                Pen pen = new Pen(Color.Red, 1);
                Pen pen1 = new Pen(Color.Blue, 1);

                CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[0].X, (int)curves.Items[CurrentCurve].Point[0].Y, Color.Blue);
                Graphics g = Graphics.FromImage(CurveOut);
                g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[0].X, (float)curves.Items[CurrentCurve].Point[0].Y, 2, 2);
                saveOldX = (float)curves.Items[CurrentCurve].Point[0].X;
                saveOldY = (float)curves.Items[CurrentCurve].Point[0].Y;
                for (uint i = 1; i < curves.Items[CurrentCurve].CountPoints; ++i)
                {
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[i].X, (int)curves.Items[CurrentCurve].Point[i].Y, Color.Blue);
                    PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y) };
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y, 2, 2);
                    g.DrawLines(pen, points);
                    saveOldX = (float)curves.Items[CurrentCurve].Point[i].X;
                    saveOldY = (float)curves.Items[CurrentCurve].Point[i].Y;
                }

                pen = new Pen(Color.Black, 1);
                pen1 = new Pen(Color.Green, 1);

                CurveOut.SetPixel((int)curves.Items[CurrentCurve].AdductionPoints[0].X, (int)curves.Items[CurrentCurve].AdductionPoints[0].Y, Color.Green);
                g = Graphics.FromImage(CurveOut);
                g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].AdductionPoints[0].X, (float)curves.Items[CurrentCurve].AdductionPoints[0].Y, 2, 2);
                saveOldX = (float)curves.Items[CurrentCurve].AdductionPoints[0].X;
                saveOldY = (float)curves.Items[CurrentCurve].AdductionPoints[0].Y;
                for (uint i = 1; i < curves.Items[CurrentCurve].AdductionCount; ++i)
                {
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].AdductionPoints[i].X, (int)curves.Items[CurrentCurve].AdductionPoints[i].Y, Color.Green);
                    PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].AdductionPoints[i].X, (float)curves.Items[CurrentCurve].AdductionPoints[i].Y) };
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].AdductionPoints[i].X, (float)curves.Items[CurrentCurve].AdductionPoints[i].Y, 2, 2);
                    g.DrawLines(pen, points);
                    saveOldX = (float)curves.Items[CurrentCurve].AdductionPoints[i].X;
                    saveOldY = (float)curves.Items[CurrentCurve].AdductionPoints[i].Y;
                }
            }
            else if (3 == DrawSelect)
            {
                for (UInt32 k = 0; k < curves.Items[CurrentCurve].ResultSegmentCount; ++k)
                {
                    float saveOldX = 0;
                    float saveOldY = 0;
                    Pen pen = new Pen(Color.Red, 1);
                    Pen pen1 = new Pen(Color.Blue, 1);
                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X, (int)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y, Color.Blue);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountOfPointsAfterSimplification[k]; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (int)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y, Color.Blue);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y;
                    }
                }
            }
            else if (4 == DrawSelect)
            {
                float saveOldX = 0;
                float saveOldY = 0;
                Pen pen = new Pen(Color.Red, 1);
                Pen pen1 = new Pen(Color.Blue, 1);
                {

                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[0].X, (int)curves.Items[CurrentCurve].Point[0].Y, Color.Blue);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[0].X, (float)curves.Items[CurrentCurve].Point[0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].Point[0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].Point[0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountPoints; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[i].X, (int)curves.Items[CurrentCurve].Point[i].Y, Color.Blue);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].Point[i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].Point[i].Y;
                    }
                }
                for (UInt32 k = 0; k < curves.Items[CurrentCurve].ResultSegmentCount; ++k)
                {
                    saveOldX = 0;
                    saveOldY = 0;
                    pen = new Pen(Color.Black, 1);
                    pen1 = new Pen(Color.Green, 1);
                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X, (int)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y, Color.Green);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountOfPointsAfterSimplification[k]; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (int)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y, Color.Green);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSimplification[k][i].Y;
                    }
                }


            }
            else if (5 == DrawSelect)
            {
                for (UInt32 k = 0; k < curves.Items[CurrentCurve].ResultSegmentCount; ++k)
                {
                    float saveOldX = 0;
                    float saveOldY = 0;
                    Pen pen = new Pen(Color.Red, 1);
                    Pen pen1 = new Pen(Color.Blue, 1);
                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X, (int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y, Color.Blue);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountOfPointsAfterSmoothing[k]; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y, Color.Blue);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y;
                    }
                }
            }
            else if (6 == DrawSelect)
            {
                float saveOldX = 0;
                float saveOldY = 0;
                Pen pen = new Pen(Color.Red, 1);
                Pen pen1 = new Pen(Color.Blue, 1);
                {

                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[0].X, (int)curves.Items[CurrentCurve].Point[0].Y, Color.Blue);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[0].X, (float)curves.Items[CurrentCurve].Point[0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].Point[0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].Point[0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountPoints; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].Point[i].X, (int)curves.Items[CurrentCurve].Point[i].Y, Color.Blue);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].Point[i].X, (float)curves.Items[CurrentCurve].Point[i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].Point[i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].Point[i].Y;
                    }
                }
                for (UInt32 k = 0; k < curves.Items[CurrentCurve].ResultSegmentCount; ++k)
                {
                    saveOldX = 0;
                    saveOldY = 0;
                    pen = new Pen(Color.Black, 1);
                    pen1 = new Pen(Color.Green, 1);
                    //System.Windows.Forms.MessageBox.Show("0 point is " + (int)curves.Items[CurrentCurve].AdductionPoints[0].X + " " + (int)curves.Items[CurrentCurve].AdductionPoints[0].Y);
                    CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X, (int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y, Color.Green);
                    Graphics g = Graphics.FromImage(CurveOut);
                    g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y, 2, 2);
                    saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].X;
                    saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][0].Y;
                    for (uint i = 1; i < curves.Items[CurrentCurve].CountOfPointsAfterSmoothing[k]; ++i)
                    {
                        CurveOut.SetPixel((int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (int)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y, Color.Green);
                        PointF[] points = { new PointF(saveOldX, saveOldY),
                    new PointF((float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y) };
                        g.DrawEllipse(pen1, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X, (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y, 2, 2);
                        g.DrawLines(pen, points);
                        saveOldX = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].X;
                        saveOldY = (float)curves.Items[CurrentCurve].PointsAfterSmoothing[k][i].Y;
                    }
                }

            }
            pictureBox1.Image = CurveOut;
            CurveOut = null;
        }

        private void openToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            SaveFileDialog savedialog = new SaveFileDialog();
            savedialog.Title = "Save image as ...";
            savedialog.OverwritePrompt = true;
            savedialog.CheckPathExists = true;
            savedialog.Filter =
                "Bitmap File(*.bmp)|*.bmp|" +
                "GIF File(*.gif)|*.gif|" +
                "JPEG File(*.jpg)|*.jpg|" +
                "TIF File(*.tif)|*.tif|" +
                "PNG File(*.png)|*.png";
            savedialog.ShowHelp = true;
            // If selected, save
            if (savedialog.ShowDialog() == DialogResult.OK)
            {
                // Get the user-selected file name
                string fileName = savedialog.FileName;
                // Get the extension
                string strFilExtn =
                    fileName.Remove(0, fileName.Length - 3);
                // Save file
                switch (strFilExtn)
                {
                    case "bmp":
                        pictureBox1.Image.Save(fileName, System.Drawing.Imaging.ImageFormat.Bmp);
                        break;
                    case "jpg":
                        pictureBox1.Image.Save(fileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                        break;
                    case "gif":
                        pictureBox1.Image.Save(fileName, System.Drawing.Imaging.ImageFormat.Gif);
                        break;
                    case "tif":
                        pictureBox1.Image.Save(fileName, System.Drawing.Imaging.ImageFormat.Tiff);
                        break;
                    case "png":
                        pictureBox1.Image.Save(fileName, System.Drawing.Imaging.ImageFormat.Png);
                        break;
                    default:
                        break;
                }
            }

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox8.Text = "";
            textBox9.Text = "";
            comboBox2.Items.Clear();
            string select = comboBox1.SelectedItem.ToString();
            char[] splitter = { ' ' };
            string[] stringAfterSplit;
            stringAfterSplit = select.Split(splitter);
            //System.Windows.Forms.MessageBox.Show(stringAfterSplit[1].ToString());
            CurrentCurve = System.Convert.ToInt32(stringAfterSplit[1]) - 1;
            textBox1.Text = curves.Items[CurrentCurve].CountPoints.ToString();


            if ((!taskBusy) && (StepForCurves[CurrentCurve] == 0))
                DoAsyncWork_RequestCurve((UInt32)CurrentCurve);

            button2.Enabled = true;
            button3.Enabled = false;
            button5.Enabled = false;
            button6.Enabled = false;
            button9.Enabled = true;

            textBox12.Text = "";
            textBox13.Text = "";
            textBox14.Text = "";
            textBox15.Text = "";
            textBox16.Text = "";
            textBox17.Text = "";
            textBox18.Text = "";

            if (curves.Measurements[CurrentCurve].adduction != 0)
            {
                textBox12.Text = curves.Measurements[CurrentCurve].adduction.ToString();
            }
            if (curves.Measurements[CurrentCurve].segmentation != 0)
            {
                textBox13.Text = curves.Measurements[CurrentCurve].segmentation.ToString();
            }
            if (curves.Measurements[CurrentCurve].simplification != 0)
            {
                textBox14.Text = curves.Measurements[CurrentCurve].simplification.ToString();
            }
            if (curves.Measurements[CurrentCurve].smoothing != 0)
            {
                textBox15.Text = curves.Measurements[CurrentCurve].smoothing.ToString();
            }
            if (curves.Measurements[CurrentCurve].overall != 0)
            {
                textBox16.Text = curves.Measurements[CurrentCurve].overall.ToString();
            }
            if (curves.Measurements[CurrentCurve].overall != 0 ||
                curves.Measurements[CurrentCurve].overall != 0)
            {
                textBox17.Text = curves.CharactMeasurements[CurrentCurve].sinuosityCoef_source.ToString();
                textBox18.Text = curves.CharactMeasurements[CurrentCurve].sinuosityCoef_result.ToString();
            }

                if (StepForCurves[CurrentCurve] > 1)
            {
                textBox2.Text = curves.Items[CurrentCurve].AdductionCount.ToString();
                button2.Enabled = false;
                button3.Enabled = true;
                button5.Enabled = false;
                button6.Enabled = false;
                button9.Enabled = false;
            }
            if (StepForCurves[CurrentCurve] > 2)
            {
                button3.Enabled = false;
                button5.Enabled = true;
                button6.Enabled = false;
                button9.Enabled = false;
            }
            if (StepForCurves[CurrentCurve] > 3)
            {
                button5.Enabled = false;
                button6.Enabled = true;
                button9.Enabled = false;
            }
            if (StepForCurves[CurrentCurve] > 4)
            {
                button6.Enabled = false;
                button9.Enabled = false;
            }
            //comboBox3.Items.Add("Yes");
            //comboBox3.Items.Add("No");
            if (PointsCanBeDrawed[CurrentCurve])
            {
                comboBox2.Items.Add("1. Only initial points");
            }
            else
            {
                if (StepForCurves[CurrentCurve] == 6)
                {
                    button2.Enabled = true;
                }
            }
            /////////////////////////////////////////////////
            if (AdductionPointsCanBeDrawed[CurrentCurve])
            {
                comboBox2.Items.Add("2. Only adduction points");
                comboBox2.Items.Add("3. Intitial points and adduction points");
            }
            else
            {
                if (StepForCurves[CurrentCurve] == 6)
                {
                    button3.Enabled = true;
                }
            }
            /////////////////////////////////////////////////
            if (SimplifactionPointsCanBeDrawed[CurrentCurve])
            {
                comboBox2.Items.Add("4. Only simplifaction points");
                comboBox2.Items.Add("5. Intitial points and simplifaction points");
            }
            else
            {
                if (StepForCurves[CurrentCurve] == 6)
                {
                    button5.Enabled = true;
                }
            }
            /////////////////////////////////////////////////
            if (SmoothingPointsCanBeDrawed[CurrentCurve])
            {
                comboBox2.Items.Add("6. Only smoothing points");
                comboBox2.Items.Add("7. Intitial points and smoothing points");
            }
            else
            {
                if (StepForCurves[CurrentCurve] == 6)
                {
                    button6.Enabled = true;
                }
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            string select = comboBox2.SelectedItem.ToString();
            char[] splitter = { '.' };
            string[] stringAfterSplit;
            stringAfterSplit = select.Split(splitter);
            DrawSelect = System.Convert.ToUInt32(stringAfterSplit[0]) - 1;
        }

        /**
        * This is 4 step
        */
        private void button5_Click(object sender, EventArgs e)
        {
            if (((-1 != CurrentCurve) && ((3 == StepForCurves[CurrentCurve]) || (3 < StepForCurves[CurrentCurve]))) ||
                (6 == StepForCurves[CurrentCurve]))
            {
                if (textBox11.Text != "")
                {
                    curves.Items[CurrentCurve].SetValueOfScale(System.Convert.ToDouble(textBox11.Text));
                }

                var requestCurvePoints = new RestRequest("/simplification_curve", Method.GET);

                requestCurvePoints.AddParameter("curve_number", CurrentCurve);
                requestCurvePoints.AddParameter("result",
                    System.Convert.ToInt32(OnlyResult[CurrentCurve]));

                IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                        + queryCurvePointsResult.StatusCode.ToString()
                        + " during handling user's request to server.");
                    return;
                }

                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfSegm = (UInt32)parsedCurveReq["count_segments"];
                UInt32[] CountOfAdductionPointsInSimplSegment = new UInt32[curveCountOfSegm];
                UInt32 totalCountOfPoints = (UInt32)parsedCurveReq["total_count_points"];

                JArray arrayOfSegm = (JArray)parsedCurveReq["segments"];
                System.Diagnostics.Debug.WriteLine(arrayOfSegm.ToString());
                UInt32 k = 0;
                Point[,] AdductionPointsInSimplSegment = new Point[curveCountOfSegm,
                                                                   curves.Items[CurrentCurve].AdductionCount];

                UInt32 segmIter = 0;

                foreach (JObject segm in arrayOfSegm)
                {
                    UInt32 count_of_points = (UInt32)segm["count_points"];
                    JArray arrayOfPoints = (JArray)segm["segment"];
                    CountOfAdductionPointsInSimplSegment[segmIter] = count_of_points;

                    foreach (JObject point in arrayOfPoints)
                    {
                        AdductionPointsInSimplSegment[segmIter, k].X = (UInt32)point["X"];
                        AdductionPointsInSimplSegment[segmIter, k].Y = (UInt32)point["Y"];
                        k++;
                    }

                    segmIter++;
                    k = 0;
                }
                Double spentTime = (Double)parsedCurveReq["time"];
                curves.SetResultOfSimplification(CurrentCurve, AdductionPointsInSimplSegment,
                    curveCountOfSegm, CountOfAdductionPointsInSimplSegment, totalCountOfPoints,
                    spentTime);


                textBox10.Text = curves.Items[CurrentCurve].TotalCountOfPointsAfterSimplification.ToString();

                textBox14.Text = curves.Measurements[CurrentCurve].simplification.ToString();
                button2.Enabled = false;
                button3.Enabled = false;
                button5.Enabled = false;
                button6.Enabled = true;
                button9.Enabled = false;
                if (6 != StepForCurves[CurrentCurve])
                {
                    StepForCurves[CurrentCurve] = 4;
                }
                if (!SimplifactionPointsCanBeDrawed[CurrentCurve])
                {
                    comboBox2.Items.Add("4. Only simplifaction points");
                    comboBox2.Items.Add("5. Intitial points and simplifaction points");
                }
                SimplifactionPointsCanBeDrawed[CurrentCurve] = true;
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("ERROR: Illegal step of Algorithm. Expected step is 3," +
                                                     ((CurrentCurve != -1) ? " but current step is " + StepForCurves[CurrentCurve].ToString() :
                                                     " but current curve is " + CurrentCurve.ToString()));
                if ((Initialize) && (CurrentCurve == -1))
                {
                    System.Windows.Forms.MessageBox.Show("Please, select curve from the list");
                }
            }
        }

        private void InitializeTypeOfStorageComboBox()
        {
            comboBox3.Items.Insert(0, "File");
            comboBox3.Items.Insert(1, "DataBase");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
            InitializeTypeOfStorageComboBox();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog opendialog = new OpenFileDialog();
            opendialog.Title = "Open file ...";
            opendialog.CheckPathExists = true;
            opendialog.ShowHelp = true;
            // If selected, save
            if (opendialog.ShowDialog() == DialogResult.OK)
            {
                // Get the user-selected file name

                filename = new string(opendialog.FileName.ToCharArray());
                
            }
        }

        /**
        * This is 5 step
        */
        private void button6_Click(object sender, EventArgs e)
        {
            if (((-1 != CurrentCurve) && ((4 == StepForCurves[CurrentCurve]) || (5 == StepForCurves[CurrentCurve])))  ||
                (6 == StepForCurves[CurrentCurve]))
            {
                if (textBox11.Text != "")
                {
                    curves.Items[CurrentCurve].SetValueOfScale(System.Convert.ToDouble(textBox11.Text));
                }

                var requestCurvePoints = new RestRequest("/smoothing_curve", Method.GET);

                requestCurvePoints.AddParameter("curve_number", CurrentCurve);
                requestCurvePoints.AddParameter("result",
                    System.Convert.ToInt32(OnlyResult[CurrentCurve]));

                IRestResponse queryCurvePointsResult = client.Execute(requestCurvePoints);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                        + queryCurvePointsResult.StatusCode.ToString()
                        + " during handling user's request to server.");
                    return;
                }

                System.Diagnostics.Debug.WriteLine(queryCurvePointsResult.Content);
                JObject parsedCurveReq = JObject.Parse(queryCurvePointsResult.Content);
                UInt32 curveCountOfSegm = (UInt32)parsedCurveReq["count_segments"];
                UInt32[] CountOfAdductionPointsInSmoothSegment = new UInt32[curveCountOfSegm];
                UInt32 totalCountOfPoints = (UInt32)parsedCurveReq["total_count_points"];
                JArray arrayOfSegm = (JArray)parsedCurveReq["segments"];
                System.Diagnostics.Debug.WriteLine(arrayOfSegm.ToString());
                UInt32 k = 0;
                Point[,] AdductionPointsInSmoothSegment = new Point[curveCountOfSegm,
                                                                    curves.Items[CurrentCurve].AdductionCount];

                UInt32 segmIter = 0;

                foreach (JObject segm in arrayOfSegm)
                {
                    UInt32 count_of_points = (UInt32)segm["count_points"];
                    JArray arrayOfPoints = (JArray)segm["segment"];
                    CountOfAdductionPointsInSmoothSegment[segmIter] = count_of_points;

                    foreach (JObject point in arrayOfPoints)
                    {
                        AdductionPointsInSmoothSegment[segmIter, k].X = (UInt32)point["X"];
                        AdductionPointsInSmoothSegment[segmIter, k].Y = (UInt32)point["Y"];
                        k++;
                    }

                    segmIter++;
                    k = 0;
                }
                Double spentTime = (Double)parsedCurveReq["time"];
                Double overallSpentTime = (Double)parsedCurveReq["overall_time"];
                Double sinuosityCoef_source = (Double)parsedCurveReq["sinuosity_coef_source"];
                Double sinuosityCoef_result = (Double)parsedCurveReq["sinuosity_coef_result"];

                curves.SetResultOfSmoothing(CurrentCurve, AdductionPointsInSmoothSegment,
                    curveCountOfSegm, CountOfAdductionPointsInSmoothSegment, totalCountOfPoints,
                    spentTime);
                curves.SetResultsOfGeneralization(CurrentCurve, overallSpentTime,
                    sinuosityCoef_source, sinuosityCoef_result);
                //curves.Items[CurrentCurve].Smoothing();
                //textBox8.Text = curves.Items[CurrentCurve].CountOfSegments.ToString();
                //textBox9.Text = curves.Items[CurrentCurve].ResultSegmentCount.ToString();
                textBox10.Text = curves.Items[CurrentCurve].TotalCountOfPointsAfterSimplification.ToString();
                button2.Enabled = false;
                button3.Enabled = false;
                button5.Enabled = false;
                button6.Enabled = false;
                button9.Enabled = false;
                textBox15.Text = curves.Measurements[CurrentCurve].smoothing.ToString();
                textBox16.Text = curves.Measurements[CurrentCurve].overall.ToString();
                textBox17.Text = curves.CharactMeasurements[CurrentCurve].sinuosityCoef_source.ToString();
                textBox18.Text = curves.CharactMeasurements[CurrentCurve].sinuosityCoef_result.ToString();

                if (6 != StepForCurves[CurrentCurve])
                {
                    StepForCurves[CurrentCurve] = 5;
                }
                if (!SmoothingPointsCanBeDrawed[CurrentCurve])
                { 
                    comboBox2.Items.Add("6. Only smoothing points");
                    comboBox2.Items.Add("7. Intitial points and smoothing points");
                }
                SmoothingPointsCanBeDrawed[CurrentCurve] = true;
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("ERROR: Illegal step of Algorithm. Expected step is 4," +
                                                     ((CurrentCurve != -1) ? " but current step is " + StepForCurves[CurrentCurve].ToString() :
                                                     " but current curve is " + CurrentCurve.ToString()));
                if ((Initialize) && (CurrentCurve == -1))
                {
                    System.Windows.Forms.MessageBox.Show("Please, select curve from the list");
                }
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            pictureBox1.Height = pictureBox1.Width = pictureBox1.Width - 100;
        }

        private void button7_Click(object sender, EventArgs e)
        {
            pictureBox1.Height = pictureBox1.Width = pictureBox1.Width + 100;
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }
        Boolean type = false;
        private void zoomableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!type)
            {
                pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
                type = true;
            }
            else
            {
                pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
                type = false;
            }
        }

        private void saveTheResultToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ((-1 != CurrentCurve) && ((4 == StepForCurves[CurrentCurve]) || (5 == StepForCurves[CurrentCurve])))
            {
                if (textBox11.Text != "")
                {
                    curves.Items[CurrentCurve].SetValueOfScale(System.Convert.ToDouble(textBox11.Text));
                }

                var requestSave = new RestRequest("/save_curve", Method.GET);

                requestSave.AddParameter("curve_number", CurrentCurve);

                IRestResponse queryCurvePointsResult = client.Execute(requestSave);
                if (queryCurvePointsResult.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    System.Windows.Forms.MessageBox.Show("An error occured "
                        + queryCurvePointsResult.StatusCode.ToString()
                        + " during handling user's request to server.");
                    return;
                }
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("ERROR: Illegal step of Algorithm. Expected step is 4," +
                                                     ((CurrentCurve != -1) ? " but current step is " + StepForCurves[CurrentCurve].ToString() :
                                                     " but current curve is " + CurrentCurve.ToString()));
                if ((Initialize) && (CurrentCurve == -1))
                {
                    System.Windows.Forms.MessageBox.Show("Please, select curve from the list");
                }
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void standaloneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            appType = AppType.STANDALONE;
        }

        private void clientToolStripMenuItem_Click(object sender, EventArgs e)
        {
            appType = AppType.CLIENT;
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if ((!taskBusy) && (StepForCurves[CurrentCurve] == 1))
                DoAsyncWork_Generalization((UInt32)CurrentCurve);
            else
                System.Windows.Forms.MessageBox.Show("Please, try again");
        }

        private void label19_Click(object sender, EventArgs e)
        {

        }

        private void textBox16_TextChanged(object sender, EventArgs e)
        {

        }

        private void benchmarkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Benchmark benchForm = new Benchmark();
            benchForm.Show();
        }
    }
}
