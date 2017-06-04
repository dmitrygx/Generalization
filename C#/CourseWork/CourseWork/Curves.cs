using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct DBInfo
{
    public string Code;
    public long Number;
}


struct TimeMeasurements
{
    public Double adduction;
    public Double segmentation;
    public Double simplification;
    public Double smoothing;
    public Double overall;
}

struct CharactMeasurements
{
    public Double sinuosityCoef_source;
    public Double sinuosityCoef_result;
}

namespace CourseWork
{
    class Curves
    {
        public TimeMeasurements[] Measurements;
        public CharactMeasurements[] CharactMeasurements;

        private File file;
        public Curve[] Items;
        public uint NumOfCurves;
        Double C = 0.5;
        private UInt32 Np = 500;
        private UInt32 Ns = 50;
        private Double f = 5;
        private UInt32 Ninit = 1000;

        public DBInfo[] CurvesMatchingScheme;


        public Curves(UInt32 numOfCurves, UInt32[] arrayOfCountPoints, Double C_, UInt32 Np_ = 500, UInt32 Ns_ = 50, Double f_ = 5, UInt32 Ninit_ = 1000)
        {
            C = C_;
            Np = Np_;
            Ns = Ns_;
            f = f_;
            Ninit = Ninit_;
            //System.Windows.Forms.MessageBox.Show("In Curves C = " + C.ToString());
            //file = new File();
            //if (filename != null)
            //{
            //    file.SetPath(filename.ToCharArray());
            //}
            //file.ParseAllDataInFile();
            Measurements = new TimeMeasurements[numOfCurves];
            CharactMeasurements = new CharactMeasurements[numOfCurves];
            Items = new Curve[/*file.NumOfCurves*/numOfCurves];
            NumOfCurves = /*file.NumOfCurves*/numOfCurves;
            //for (uint i = 0; i < NumOfCurves; i++)
            //{
            //    Items[i] = new Curve(C, Np, Ns, f, Ninit);
            //    Items[i].BuildCurve(file.PointInCurves[i]);
            //    for (uint k = 0; k < Items[i].CountPoints; k++)
            //    {
            //        Items[i].Point[k].X = (int)file.CurvesX[i][k];
            //        Items[i].Point[k].Y = (int)file.CurvesY[i][k];
            //    }
            //}
            for (UInt32 i = 0; i < numOfCurves; i++)
            {
                Measurements[i].adduction = 0;
                Measurements[i].segmentation = 0;
                Measurements[i].simplification = 0;
                Measurements[i].smoothing = 0;
                Measurements[i].overall = 0;
                CharactMeasurements[i].sinuosityCoef_source = 0;
                CharactMeasurements[i].sinuosityCoef_result = 0;
                Items[i] = new Curve(C, Np, Ns, f, Ninit);
                Items[i].BuildCurve(arrayOfCountPoints[i]);
            }
        }
        public void InitializaCurves(Point[][] Points)
        {
            for (uint i = 0; i < NumOfCurves; i++)
            {
                for (uint k = 0; k < Items[i].CountPoints; k++)
                {
                    Items[i].Point[k].X = (int)Points[i][k].X;
                    Items[i].Point[k].Y = (int)Points[i][k].Y;
                }
            }
        }

        public void InitializeCurve(Point [] Points, UInt32 CurveSerialNumber)
        {
            for (uint k = 0; k < Items[CurveSerialNumber].CountPoints; k++)
            {
                Items[CurveSerialNumber].Point[k].X = (int)Points[k].X;
                Items[CurveSerialNumber].Point[k].Y = (int)Points[k].Y;
            }
        }

        public void SetResultOfAdduction(int CurveNumber, Point[] Points, UInt32 Count, Double spentTime)
        {
            Items[CurveNumber].AdductionPoints = new Point[Count];
            Items[CurveNumber].AdductionCount = Count;
            for (UInt32 i = 0; i < Count; i++)
            {
                Items[CurveNumber].AdductionPoints[i].X = (int)Points[i].X;
                Items[CurveNumber].AdductionPoints[i].Y = (int)Points[i].Y;
            }

            Measurements[CurveNumber].adduction = spentTime;
        }

        delegate UInt32 FindMax(UInt32 Count, UInt32[] CountOfPoints);
        public void SetResultOfSegmentation(int CurveNumber, Point[,] Points,
            UInt32 Count, UInt32 InitialCount, UInt32[] CountOfAdductionPointsInSegment,
            Double spentTime)
        {
            Items[CurveNumber].ResultSegmentCount = Count;
            Items[CurveNumber].CountOfSegments = InitialCount;
            Items[CurveNumber].CountOfAdductionPointsInSegment = new UInt32[Count];

            FindMax fm = (cnt, cntOfPoints) =>
            {
                UInt32 maxVal = 0;
                for (UInt32 i = 0; i < cnt; i++)
                {
                    if (maxVal < cntOfPoints[i])
                        maxVal = cntOfPoints[i];
                }
                return maxVal;
            };

            Items[CurveNumber].AdductionPointsInSegment =
                new Point[Count, fm(Count, CountOfAdductionPointsInSegment)];

            for (UInt32 i = 0; i < Count; i++)
            {
                Items[CurveNumber].CountOfAdductionPointsInSegment[i] =
                    CountOfAdductionPointsInSegment[i];
                for (UInt32 j = 0; j < Items[CurveNumber].CountOfAdductionPointsInSegment[i]; j++)
                {
                    Items[CurveNumber].AdductionPointsInSegment[i, j].X = (int)Points[i, j].X;
                    Items[CurveNumber].AdductionPointsInSegment[i, j].Y = (int)Points[i, j].Y;
                }
            }

            Measurements[CurveNumber].segmentation = spentTime;
        }

        public void SetResultOfSimplification(int CurveNumber, Point[,] Points,
            UInt32 Count, UInt32[] CountOfAdductionPointsInSimplSegment, UInt32 TotalCountOfPoints,
            Double spentTime)
        {
            Items[CurveNumber].CountOfPointsAfterSimplification = new UInt32[Count];
            Items[CurveNumber].PointsAfterSimplification =
                new Point[Count][];
            //Items[CurveNumber].TotalCountOfPointsAfterSimplification = 0;
            for (UInt32 i = 0; i < Count; i++)
            {
                Items[CurveNumber].CountOfPointsAfterSimplification[i] =
                    CountOfAdductionPointsInSimplSegment[i];
                Items[CurveNumber].PointsAfterSimplification[i] =
                    new Point[CountOfAdductionPointsInSimplSegment[i]];
                //Items[CurveNumber].TotalCountOfPointsAfterSimplification +=
                //    Items[CurveNumber].CountOfPointsAfterSimplification[i];
                for (UInt32 j = 0; j < Items[CurveNumber].CountOfPointsAfterSimplification[i]; j++)
                {
                    Items[CurveNumber].PointsAfterSimplification[i][j].X = (int)Points[i, j].X;
                    Items[CurveNumber].PointsAfterSimplification[i][j].Y = (int)Points[i, j].Y;
                }
            }
            Items[CurveNumber].TotalCountOfPointsAfterSimplification = TotalCountOfPoints;
            Measurements[CurveNumber].simplification = spentTime;
        }

        public void SetResultOfSmoothing(int CurveNumber, Point[,] Points,
            UInt32 Count, UInt32[] CountOfAdductionPointsInSmoothSegment, UInt32 TotalCountOfPoints,
            Double spentTime)
        {
            Items[CurveNumber].CountOfPointsAfterSmoothing = new UInt32[Count];
            Items[CurveNumber].PointsAfterSmoothing =
                new Point[Count][];
            //Items[CurveNumber].TotalCountOfPointsAfterSmoothing = 0;
            for (UInt32 i = 0; i < Count; i++)
            {
                Items[CurveNumber].CountOfPointsAfterSmoothing[i] =
                    CountOfAdductionPointsInSmoothSegment[i];
                Items[CurveNumber].PointsAfterSmoothing[i] =
                    new Point[CountOfAdductionPointsInSmoothSegment[i]];
                //Items[CurveNumber].TotalCountOfPointsAfterSmoothing +=
                //    Items[CurveNumber].CountOfPointsAfterSmoothing[i];
                for (UInt32 j = 0; j < Items[CurveNumber].CountOfPointsAfterSmoothing[i]; j++)
                {
                    Items[CurveNumber].PointsAfterSmoothing[i][j].X = (int)Points[i, j].X;
                    Items[CurveNumber].PointsAfterSmoothing[i][j].Y = (int)Points[i, j].Y;
                }
            }
            Items[CurveNumber].TotalCountOfPointsAfterSmoothing = TotalCountOfPoints;
            Measurements[CurveNumber].smoothing = spentTime;
        }

        public void SetResultsOfGeneralization(int CurveNumber, Double spentTime,
            Double sinuosityCoef_source, Double sinuosityCoef_result)
        {
            Measurements[CurveNumber].overall = spentTime;
            CharactMeasurements[CurveNumber].sinuosityCoef_source = sinuosityCoef_source;
            CharactMeasurements[CurveNumber].sinuosityCoef_result = sinuosityCoef_result;
        }

        public void Write()
        {
            file.Write(Items, NumOfCurves);
        }
        public void SetPathForFile(char[] string_filename)
        {
            file.SetPath(string_filename);
        }
    }
}
