using Newtonsoft.Json.Linq;
using RestSharp;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CourseWork
{
    public partial class Benchmark : Form
    {
        RestClient client;
        Double C = 0.5;
        private UInt32 Np = 500;
        private UInt32 Ns = 50;
        private Double f = 5;
        private UInt32 Ninit = 1000;
        private double M = 1;
        private int OpenMP = 0;
        private int IntelMKL = 0;
        public Benchmark()
        {
            InitializeComponent();
        }

        System.Windows.Forms.Form parentForm = System.Windows.Forms.Application.OpenForms["Form1"];

        private void button1_Click(object sender, EventArgs e)
        {
            client = new RestClient("http://localhost/generalization_server");

            OpenMP = (((Form1)parentForm).checkBox1.Checked ? 1 : 0);
            IntelMKL = (((Form1)parentForm).checkBox2.Checked ? 1 : 0);
            C = 0.5;
            Np = 500;
            Ns = 50;
            f = 5;
            Ninit = 1000;
            M = 1;
            if (((Form1)parentForm).textBox3.Text != "")
            {
                ((Form1)parentForm).textBox3.Text.Replace('.', ',');
                C = System.Convert.ToDouble(((Form1)parentForm).textBox3.Text);
                //System.Windows.Forms.MessageBox.Show("C = " + C.ToString());
            }
            if (((Form1)parentForm).textBox4.Text != "")
            {
                Np = System.Convert.ToUInt32(((Form1)parentForm).textBox4.Text);
                //System.Windows.Forms.MessageBox.Show("Np = " + Np.ToString());
            }
            if (((Form1)parentForm).textBox5.Text != "")
            {
                Ns = System.Convert.ToUInt32(((Form1)parentForm).textBox5.Text);
                //System.Windows.Forms.MessageBox.Show("Ns = " + Ns.ToString());
            }
            if (((Form1)parentForm).textBox6.Text != "")
            {
                f = System.Convert.ToDouble(((Form1)parentForm).textBox6.Text);
                //System.Windows.Forms.MessageBox.Show("f = " + f.ToString());
            }
            if (((Form1)parentForm).textBox7.Text != "")
            {
                Ninit = System.Convert.ToUInt32(((Form1)parentForm).textBox7.Text);
                //System.Windows.Forms.MessageBox.Show("Ninit = " + Ninit.ToString());
            }
            if (((Form1)parentForm).textBox11.Text != "")
            {
                M = System.Convert.ToDouble(((Form1)parentForm).textBox11.Text);
            }

            var request = new RestRequest("/benchmark", Method.GET);
            UInt32 pps = (textBox1.Text != "" ? System.Convert.ToUInt32(textBox1.Text) : 100);
            UInt32 min_num_seg = (textBox2.Text != "" ? System.Convert.ToUInt32(textBox2.Text) : 1);
            UInt32 max_num_seg = (textBox3.Text != "" ? System.Convert.ToUInt32(textBox3.Text) : 1);

            request.AddParameter("points_per_seg", pps);
            request.AddParameter("min_seg_cnt", min_num_seg);
            request.AddParameter("max_seg_cnt", max_num_seg);
            request.AddParameter("params_C", C);
            request.AddParameter("params_Np", Np);
            request.AddParameter("params_Ns", Ns);
            request.AddParameter("params_f", f);
            request.AddParameter("params_Ninit", Ninit);
            request.AddParameter("params_M", M);

            request.AddParameter("params_OpenMP", OpenMP);
            request.AddParameter("params_IntelMKL", IntelMKL);


            IRestResponse queryResult = client.Execute(request);
            if (queryResult.StatusCode != System.Net.HttpStatusCode.OK)
            {
                System.Windows.Forms.MessageBox.Show("An error occured "
                    + queryResult.StatusCode.ToString()
                    + " during handling user's request to server.");
                return;
            }

            JObject parsedReq = JObject.Parse(queryResult.Content);

            JArray array = (JArray)parsedReq["timers"];

            System.Diagnostics.Debug.WriteLine(array.ToString());

            Double[] simpl_time = new Double[array.Count];
            Double[] smooth_time = new Double[array.Count];
            UInt32[] seg_num = new UInt32[array.Count];
            UInt32 count = 0;
            UInt32 iter_seg_num = min_num_seg;

            foreach (JObject arrayObj in array)
            {
                seg_num[count] = iter_seg_num++;

                Double parsed_simpl_time = (Double)arrayObj["simpl_time"];
                simpl_time[count] = parsed_simpl_time;

                Double parsed_smooth_time = (Double)arrayObj["smooth_time"];
                smooth_time[count] = parsed_smooth_time;

                count++;
                //System.Windows.Forms.MessageBox.Show(parsedCount.ToString());
            }

            DataTable dt = new DataTable();
            dt.Columns.Add(new DataColumn("Number of segments", typeof(UInt32)));
            dt.Columns.Add(new DataColumn("Simplification", typeof(string)));
            dt.Columns.Add(new DataColumn("Smoothing", typeof(string)));

            for (UInt32 i = 0; i < count; i++)
            {
                String simpl = simpl_time[i].ToString("F9");
                String smooth = smooth_time[i].ToString("F9");
                dt.Rows.Add(seg_num[i], simpl, smooth);
            }

            dataGridView1.DataSource = dt;
        }
    }
}
