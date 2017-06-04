using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CourseWork
{
    class File
    {
        private string path;
        private string pathRes;
        public File()
        {
            path = "C:\\Users\\Дмитрий\\Documents\\Visual Studio 2015\\Projects\\CourseWork\\CourseWork\\bin\\Debug\\coursework_big.txt";
            pathRes = "C:\\Users\\Дмитрий\\Documents\\Visual Studio 2015\\Projects\\CourseWork\\CourseWork\\bin\\Debug\\coursework_big_res.txt";
        }
        public void SetPath(char[] string_path)
        {
            path = new string(string_path);
        }
        public List<string> Read()
        {
            return System.IO.File.ReadLines(path).ToList<string>();
        }
        public void Write(Curve[] curve, uint numOfCurves)
        {
            List<string> ListOfCurves = new List<string>();
            string str = numOfCurves.ToString();
            bool first = false;
            ListOfCurves.Add(str);
            for (uint i = 0; i < numOfCurves; i++)
            {
                uint iter = 0;
                uint maxIterate = curve[i].CountPoints % 5 == 0 ? curve[i].CountPoints / 5 : curve[i].CountPoints / 5 + 1;
                if (!first)
                {
                    str = curve[i].CountPoints.ToString();
                    ListOfCurves.Add(str);
                    first = true;
                }
                for (uint k = 0; k < maxIterate; k++)
                {
                    str = "M:(" + (1 + k * 5).ToString() + ")";
                    int j = 0;
                    if ((1 + k * 5) + 5 < curve[i].CountPoints)
                    {
                        while (j < 5)
                        {
                            str += " " + curve[i].Point[iter].X + " " + curve[i].Point[iter].Y;
                            iter++;
                            j++;
                        }
                    }
                    else
                    {
                        uint max = curve[i].CountPoints - (1 + k * 5) + 1;
                        while (max > 0)
                        {
                            str += " " + curve[i].Point[iter].X + " " + curve[i].Point[iter].Y;
                            iter++;
                            max--;
                        }
                    }

                    ListOfCurves.Add(str);
                }
                first = false;
            }
            System.IO.File.WriteAllLines(pathRes, ListOfCurves);
        }
        public LinkedList<string> CordinatesList;
        private List<string> AuxList;
        public uint NumOfCurves = 0;
        public uint[] PointInCurves;
        public Double[][] CurvesX;
        public Double[][] CurvesY;
        public LinkedList<string> ParseAllDataInFile()
        {
            char separator = ' ';
            AuxList = System.IO.File.ReadLines(path).ToList<string>();
            NumOfCurves = Convert.ToUInt32(AuxList.First<string>());
            //System.Windows.Forms.MessageBox.Show(NumOfCurves.ToString());
            PointInCurves = new uint[NumOfCurves];
            CurvesX = new Double[NumOfCurves][];
            CurvesY = new Double[NumOfCurves][];
            int k = -1;
            int X = 0;
            int Y = 0;
            bool first = false;
            foreach (string str in AuxList)
            {
                if (!first)
                {
                    first = true;
                    continue;
                }
                if (str[0] != 'M')
                {
                    X = 0;
                    Y = 0;
                    k++;
                    PointInCurves[k] = Convert.ToUInt32(str);
                    CurvesX[k] = new Double[PointInCurves[k]];
                    CurvesY[k] = new Double[PointInCurves[k]];
                    
                    //System.Windows.Forms.MessageBox.Show(str);
                }
                else if (str[0] == 'M')
                {
                    string[] RawCord = str.Split(separator);
                    int AuxK = 3;
                    string AuxStr = "";
                    //System.Windows.Forms.MessageBox.Show(str);
                    while (RawCord[0][AuxK] != ')')
                    {
                        AuxStr += RawCord[0][AuxK];
                        AuxK++;
                    }

                    uint count = Convert.ToUInt32(AuxStr) + 5 < PointInCurves[k] ? 5 * 2 + 1: (PointInCurves[k] - Convert.ToUInt32(AuxStr) + 1) * 2 + 1;

                    for (uint i = 1; i < count; i++)
                    {
                        if (i % 2 != 0)
                        {
                            CurvesX[k][X] = Convert.ToInt32(RawCord[i]);
                            X++;
                        }
                        else
                        {
                            CurvesY[k][Y] = Convert.ToInt32(RawCord[i]);
                            //System.Windows.Forms.MessageBox.Show(CurvesY[k][Y].ToString());
                            Y++;
                        }
                    }
                }
            }
            return null;
        }
    }
}
