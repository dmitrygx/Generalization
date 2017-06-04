using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RestSharp;

namespace CourseWork
{
    struct Point
    {
        public double X;
        public double Y;
    }
    class Curve
    {
        String dbCode;
        String dbNumber;
        public UInt32 CountPoints;
        //private int[] PointsX;
        //private int[] PointsY;
        public Point[] Point;
        public Point[] AdductionPoints;
        public UInt32 AdductionCount;
        private Double[] Distance;
        public Double AvarageDistnace;
        public Double Radius;
        public Double C = 0.5;
        public Double M = 0;
        private Double[] Dm;

        Point[,] InitSegments;
        UInt32[] SegmentCountPoints;
        public UInt32 CountOfSegments;

        public UInt32[] LocalMax;
        public Double[] IntegralСharact;

        public Point[,] AdductionPointsInSegment;
        public UInt32[] CountOfAdductionPointsInSegment;
        public UInt32 ResultSegmentCount;

        public Point[][] PointsAfterSimplification;
        public UInt32[] CountOfPointsAfterSimplification;
        public UInt32 TotalCountOfPointsAfterSimplification;

        public Point[][] PointsAfterSmoothing;
        public UInt32[] CountOfPointsAfterSmoothing;
        public UInt32 TotalCountOfPointsAfterSmoothing;

        private UInt32 Ninit = 1000;
        private UInt32 Np = 500;
        private UInt32 Ns = 50;
        private Double f = 5;

        private Double[] AngularCoeffRegresLine;

        public Curve(Double C_ = 0.5, UInt32 Np_ = 500, UInt32 Ns_ = 50, Double f_ = 5, UInt32 Ninit_ = 1000)
        {
            C = C_;
            Np = Np_;
            Ns = Ns_;
            f = f_;
            Ninit = Ninit_;
        }
        public void BuildCurve(uint _CountOfPoints)
        {
            if (_CountOfPoints == 0)
                return;
            CountPoints = _CountOfPoints;
            Point = new Point[CountPoints];
            /*if (null == Point[0])
            {*/
            /*}*/
            Distance = new Double[CountPoints - 1];
            AvarageDistnace = 0;
            Radius = 0;

            AdductionPoints = new Point[CountPoints * 4];
        }
        private Double ComputeDistances()
        {
            Double sum = 0;
            for (UInt32 i = 0; i < CountPoints - 1; i++)
            {
                Distance[i] = Math.Sqrt(Math.Pow((Point[i + 1].X - Point[i].X), 2) +
                                        Math.Pow((Point[i + 1].Y - Point[i].Y), 2));

                sum += Distance[i];
            }
            return sum;
        }
        private Double ComputeAvarageDistance()
        {
            Double sum = ComputeDistances();
            return sum / (CountPoints - 1);
        }
        private Double ComputeRadius(Double AvaregeDistance)
        {
            return C * ComputeAvarageDistance();
        }
        private bool BelongPoints(Point point1, Point point2, Point current)
        {
            bool first = false;
            // 1st Quadrunt
            if (((current.X >= point1.X) && (current.X <= point2.X)) &&
                ((current.Y >= point1.Y) && (current.Y <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("1");
                first = true;
            }
            // 4th Quadrunt
            if (((current.X >= point1.X) && (current.X <= point2.X)) &&
                ((current.Y <= point1.Y) && (current.Y >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("4");
                first = true;
            }
            //3rd Quadrunt
            if (((current.X <= point1.X) && (current.X >= point2.X)) &&
                ((current.Y <= point1.Y) && (current.Y >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("3");
                first = true;
            }
            //2st Quadrunt 
            if (((current.X <= point1.X) && (current.X >= point2.X)) &&
                ((current.Y >= point1.Y) && (current.Y <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("2");
                first = true;
            }
            return first;
        }
        private Point[] CheckInterSection(Point point1, Point point2, double radius, Point pointCircle)
        {
            Double x1 = point1.X;
            Double x2 = point2.X;
            Double y1 = point1.Y;
            Double y2 = point2.Y;
            Double k = -((y1 - y2) / (x2 - x1));
            Double b = -((x1 * y2 - x2 * y1) / (x2 - x1));
            Double D = (Math.Pow((2 * k * b - 2 * pointCircle.X - 2 * pointCircle.Y * k), 2) - (4 + 4 * k * k) * (b * b - radius * radius + pointCircle.X * pointCircle.X + pointCircle.Y * pointCircle.Y - 2 * pointCircle.Y * b));
            if (D < 0)
            {
                //System.Windows.Forms.MessageBox.Show("D (" + D.ToString() + ") < 0");
                return null;
            }
            Double X1 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) - Math.Sqrt(D)) / (2 + 2 * k * k));
            Double X2 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) + Math.Sqrt(D)) / (2 + 2 * k * k));
            Double Y1 = k * X1 + b;
            Double Y2 = k * X2 + b;
            if (X1 == X2)
            {
                //System.Windows.Forms.MessageBox.Show(" Одна точка пересечения " + X1.ToString() + " " + Y1.ToString());
            }
            else
            {
                //System.Windows.Forms.MessageBox.Show(" Две точки пересечения " + X1.ToString() + " " + Y1.ToString() + " и " + X2.ToString() + " " + Y2.ToString());
            }
            // First root
            bool first = false;
            // 1st Quadrunt
            if (((X1 >= point1.X) && (X1 <= point2.X)) &&
                ((Y1 >= point1.Y) && (Y1 <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("1");
                first = true;
            }
            // 4th Quadrunt
            if (((X1 >= point1.X) && (X1 <= point2.X)) &&
                ((Y1 <= point1.Y) && (Y1 >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("4");
                first = true;
            }
            //3rd Quadrunt
            if (((X1 <= point1.X) && (X1 >= point2.X)) &&
                ((Y1 <= point1.Y) && (Y1 >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("3");
                first = true;
            }
            //2st Quadrunt 
            if (((X1 <= point1.X) && (X1 >= point2.X)) &&
                ((Y1 >= point1.Y) && (Y1 <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("2");
                first = true;
            }

            // Second root
            bool second = false;
            // 1st Quadrunt
            if (((X2 >= point1.X) && (X2 <= point2.X)) &&
                ((Y2 >= point1.Y) && (Y2 <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("1");
                second = true;
            }
            // 4th Quadrunt
            if (((X2 >= point1.X) && (X2 <= point2.X)) &&
                ((Y2 <= point1.Y) && (Y2 >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("4");
                second = true;
            }
            //3rd Quadrunt
            if (((X2 <= point1.X) && (X2 >= point2.X)) &&
                ((Y2 <= point1.Y) && (Y2 >= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("3");
                second = true;
            }
            //2st Quadrunt 
            if (((X2 <= point1.X) && (X2 >= point2.X)) &&
                ((Y2 >= point1.Y) && (Y2 <= point2.Y)))
            {
                //System.Windows.Forms.MessageBox.Show("2");
                second = true;
            }
            if (first)
            {
                Point[] res = new Point[1];
                res[0].X = X1;
                res[0].Y = Y1;
                return res;
            }
            if (second)
            {
                Point[] res = new Point[1];
                res[0].X = X2;
                res[0].Y = Y2;
                return res;
            }
            return null;
        }
        public void Adduction()
        {
            Radius = ComputeRadius(ComputeAvarageDistance());

            //System.Windows.Forms.MessageBox.Show(Radius.ToString());
            Point[] AdductionPointsTmp = new Point[CountPoints];
            AdductionPointsTmp[0] = Point[0];
            AdductionPoints[0] = AdductionPointsTmp[0];
            UInt32 count = 1;
            UInt32 k = 1;
            /*for (UInt32 i = 0; i < CountPoints; i++)*/
            UInt32 i = 0;
            {
                ///////////////////////////////////////////////////
                //System.Windows.Forms.MessageBox.Show(ComparePoints(Point[i], Point[i + 1], AdductionPoints[i]).ToString());
                while (true)
                {
                    Point[] res;
                    res = CheckInterSection(AdductionPoints[count - 1], Point[i + k], Radius, AdductionPoints[count - 1]);
                    if (null == res)
                    {
                        k++;
                        //System.Windows.Forms.MessageBox.Show("i = " + i.ToString() + " k = " + k.ToString());
                    }
                    else
                    {
                        AdductionPointsTmp[i] = res[0];
                        AdductionPoints[count] = AdductionPointsTmp[i];
                        //System.Windows.Forms.MessageBox.Show(count.ToString() + ". AdductionPoint[] = " + AdductionPoints[count].X + " " + AdductionPoints[count].Y);
                        count++;
                        i = i + k - 1;
                        k = 1;
                    }
                    if (i + k > CountPoints - 1)
                    {
                        AdductionPoints[count] = Point[CountPoints - 1];
                        count++;
                        break;
                    }
                }
                AdductionCount = count;
                //////////////////////////////////////////////////
                /*Point[] res; // мы всегда смотрим только след. точку, но не смотрим на свою точку
                res = CheckInterSection(Point[i], Point[i+1], Radius, AdductionPointsTmp[i]); // неправильно, необходимо учитывать, что мы будем смотреть другие
                if (null == res)
                {
                    UInt32 k = 2;
                    while (res == null)
                    {
                        res = CheckInterSection(Point[i], Point[i + k], Radius, AdductionPointsTmp[i]); // неверно, fix me. нужно себя дальше просматривать, а я дальше двигаюсь
                        k++;
                    }
                    i = i + k - 1;
                    AdductionPointsTmp[i + 1] = res[0];
                    AdductionPoints[count] = AdductionPointsTmp[i + 1];
                    //System.Windows.Forms.MessageBox.Show("AdductionPoints[count] = " 
                      //                                   + count.ToString() + " " + (AdductionPoints[count].X).ToString() 
                        //                                 + "-" + (AdductionPoints[count].Y).ToString());
                    count++;
                }
                else
                {
                    AdductionPointsTmp[i + 1] = res[0];
                    AdductionPoints[count] = AdductionPointsTmp[i + 1];
                    //System.Windows.Forms.MessageBox.Show("AdductionPoints[count] = "
                          //                               + count.ToString() + " " + (AdductionPoints[count].X).ToString()
                            //                             + "-" + (AdductionPoints[count].Y).ToString());
                    count++;
                }*/
            }
            //for (UInt32 ii = 0; ii < count; ii++)
            //{
            //    if (ii + 1 == count)
            //    System.Windows.Forms.MessageBox.Show(AdductionPoints[ii].X.ToString() + " " + AdductionPoints[ii].Y.ToString());
            //}
            ////System.Windows.Forms.MessageBox.Show("Count of points afer adduction = " + AdductionCount.ToString());
        }
        public void Segmentation()
        {
            Double[] Dist = new Double[AdductionCount - 1];
            for (UInt32 i = 0; i < AdductionCount - 1; i++)
            {
                Dist[i] = Math.Sqrt(Math.Pow((AdductionPoints[i + 1].X - AdductionPoints[i].X), 2) +
                                    Math.Pow((AdductionPoints[i + 1].Y - AdductionPoints[i].Y), 2));
            }
            CountOfSegments = AdductionCount / Ninit;
            bool flag = false;
            if ((AdductionCount % Ninit) != 0)
            {
                flag = true;
                CountOfSegments++;
            }
            InitSegments = new Point[CountOfSegments, Ninit];
            SegmentCountPoints = new UInt32[CountOfSegments];

            UInt32 k = 0;
            for (UInt32 i = 0; i < CountOfSegments; i++)
            {
                UInt32 N = Ninit;
                if ((flag) && (i == CountOfSegments - 1))
                {
                    N = AdductionCount % Ninit;
                }
                for (UInt32 j = 0; j < N; j++)
                {
                    InitSegments[i, j] = AdductionPoints[k++];
                }
                SegmentCountPoints[i] = N;
            }
            k = 0;
            Double[,] AngleOfRotation = new Double[CountOfSegments, Ninit];
            Double[] RotationOfSegment = new Double[CountOfSegments];
            Double[] FullRotationOfSegment = new Double[CountOfSegments];
            IntegralСharact = new Double[CountOfSegments];
            for (UInt32 i = 0; i < CountOfSegments; i++)
            {
                UInt32 N = Ninit;
                if ((flag) && (i == CountOfSegments - 1))
                {
                    N = AdductionCount % Ninit;
                }
                for (UInt32 j = 1; j < N - 1; j++)
                {
                    Double topLeft = (InitSegments[i, j + 1].X - InitSegments[i, j].X) *
                                     (InitSegments[i, j].X - InitSegments[i, j - 1].X);
                    Double topRight = (InitSegments[i, j + 1].Y - InitSegments[i, j].Y) *
                                     (InitSegments[i, j].Y - InitSegments[i, j - 1].Y);
                    Double botttom = Math.Pow(Dist[k++], 2);
                    Double Value = (topLeft + topRight) / botttom;
                    if (Value > 1)
                    {
                        Value = 1;
                    }
                    AngleOfRotation[i, j] = Math.Acos(Value);
                    RotationOfSegment[i] += AngleOfRotation[i, j];
                }
                FullRotationOfSegment[i] = RotationOfSegment[i] / (2 * Math.PI); /* Wi */
            }
            //System.Windows.Forms.MessageBox.Show("Count of Segments = " + CountOfSegments.ToString());

            LocalMax = new UInt32[CountOfSegments]; /* Ei */

            for (UInt32 i = 0; i < CountOfSegments; i++)
            {
                LocalMax[i] = 0;
                for (UInt32 j = 1; j < CountOfSegments - 1; j++)
                {
                    if ((AngleOfRotation[i, j] > AngleOfRotation[i, j - 1]) &&
                        (AngleOfRotation[i, j] > AngleOfRotation[i, j + 1]))
                    {
                        LocalMax[i]++;
                    }
                }
                //System.Windows.Forms.MessageBox.Show(LocalMax[i].ToString());
                IntegralСharact[i] = FullRotationOfSegment[i] + f * LocalMax[i]; /* Mi */
                //System.Windows.Forms.MessageBox.Show("Intergral Characteristic = " + IntegralСharact[i].ToString() + "Count of Points in Segment = " + SegmentCountPoints[i]);
            }
            UInt32[] MinSegmentCountPoint = new UInt32[CountOfSegments];
            UInt32 MinCountSegment = 0;
            bool ShouldUnionSegment = false;
            //System.Windows.Forms.MessageBox.Show("We have " + Ns + " " + Np);
            for (UInt32 i = 0; i < CountOfSegments; ++i)
            {
                if (SegmentCountPoints[i] < Np)
                {
                    MinSegmentCountPoint[MinCountSegment++] = i;
                    //System.Windows.Forms.MessageBox.Show("GET " + Ns + " " + Np);
                }
            }
            if ((CountOfSegments > Ns) && (MinCountSegment != 0))
            {
                ShouldUnionSegment = true;
            }
            AdductionPointsInSegment = new Point[CountOfSegments, CountPoints];
            CountOfAdductionPointsInSegment = new UInt32[CountOfSegments];
            for (UInt32 i = 0; i < CountOfSegments; ++i)
            {
                CountOfAdductionPointsInSegment[i] = 0;
            }
            ResultSegmentCount = 0;
            while (ShouldUnionSegment)
            {
                //System.Windows.Forms.MessageBox.Show("Should = " + ShouldUnionSegment.ToString());
                for (UInt32 i = 0; i < CountOfSegments; i += 2)
                {
                    if (i + 1 != CountOfSegments)
                    {
                        if ((IntegralСharact[i + 1] - IntegralСharact[i] >= -10) && (IntegralСharact[i + 1] - IntegralСharact[i] <= 10))
                        {
                            for (UInt32 j = 0; j < SegmentCountPoints[i]; ++j)
                            {
                                AdductionPointsInSegment[ResultSegmentCount, CountOfAdductionPointsInSegment[ResultSegmentCount]++] = InitSegments[i, j];
                            }
                            for (UInt32 j = 0; j < SegmentCountPoints[i + 1]; ++j)
                            {
                                AdductionPointsInSegment[ResultSegmentCount, CountOfAdductionPointsInSegment[ResultSegmentCount]++] = InitSegments[i + 1, j];
                            }
                            ResultSegmentCount++;
                        }
                        else
                        {
                            for (UInt32 j = 0; j < SegmentCountPoints[i]; ++j)
                            {
                                AdductionPointsInSegment[ResultSegmentCount, CountOfAdductionPointsInSegment[ResultSegmentCount]++] = InitSegments[i, j];
                            }
                            ResultSegmentCount++;
                            for (UInt32 j = 0; j < SegmentCountPoints[i + 1]; ++j)
                            {
                                AdductionPointsInSegment[ResultSegmentCount, CountOfAdductionPointsInSegment[ResultSegmentCount]++] = InitSegments[i + 1, j];
                            }
                            ResultSegmentCount++;
                        }
                    }
                }
                //System.Windows.Forms.MessageBox.Show("ResultSegmentCount = " + ResultSegmentCount.ToString());
                break;
            }
            if (!ShouldUnionSegment)
            {
                for (UInt32 i = 0; i < CountOfSegments; ++i)
                {
                    for (UInt32 j = 0; j < SegmentCountPoints[i]; ++j)
                    {
                        AdductionPointsInSegment[ResultSegmentCount, CountOfAdductionPointsInSegment[ResultSegmentCount]++] = InitSegments[i, j];
                    }
                    ResultSegmentCount++;
                }
            }
        }
        public void SetValueOfScale(Double m)
        {
            M = m;
        }
        private Double func1(Point p1, Point p2, Point point)
        {
            return ((p1.Y - p2.Y) * point.X + (p1.X - p2.X) * point.Y + (p1.X * p2.Y - p2.X * p1.Y));
        }
        //private Boolean IntersectionLineAndSquare(Double A, Double B, Double C, Double C1, Double C2, Double C3, Double C4, Double dist)
        //{
        //    Double Bound1 = C1 * dist;
        //    Double Bound2 = C2 * dist;
        //    Double Bound3 = C3 * dist;
        //    Double Bound4 = C4 * dist;
        //    Double[] X = new Double[4];
        //    Double[] Y = new Double[4];
        //    X[0] = C1 * B; Y[0] = -A * C1;
        //    X[1] = C2 * B; Y[1] = -A * C2;
        //    X[2] = C3 * B; Y[2] = -A * C3;
        //    X[3] = C4 * B; Y[3] = -A * C4;
        //    for (UInt32 i = 0; i < 4; ++i)
        //    {
        //        if (((X[i] >= Bound1) && (X[i] <= Bound3)) && ((Y[i] >= Bound2) && (Y[i] <= Bound4)))
        //        {
        //            return true;
        //        }
        //    }
        //    return false;

        //}
        private Boolean IntersectionLineAndSquare(Point p1, Point p2, Point[] points)
        {
            if (func1(p1, p2, points[0]) * func1(p1, p2, points[1]) >= 0)
            {
                return true;
            }
            else if (func1(p1, p2, points[1]) * func1(p1, p2, points[2]) >= 0)
            {
                return true;
            }
            else if (func1(p1, p2, points[2]) * func1(p1, p2, points[3]) >= 0)
            {
                return true;
            }
            else if (func1(p1, p2, points[3]) * func1(p1, p2, points[0]) >= 0)
            {
                return true;
            }
            return false;
        }
  
        public UInt32 ComputeQuadrics(UInt32 CurrentSegment, Double dist)
        {
            UInt32 CountOfSquare = 0;
            Boolean[,] Grid = new Boolean[10000, 10000];
            for (UInt32 k = 0; k < CountOfAdductionPointsInSegment[CurrentSegment] - 1; ++k)
            {
                Double A = AdductionPointsInSegment[CurrentSegment, k].Y - AdductionPointsInSegment[CurrentSegment, k + 1].Y;
                Double B = AdductionPointsInSegment[CurrentSegment, k + 1].X - AdductionPointsInSegment[CurrentSegment, k].X;
                Double C = AdductionPointsInSegment[CurrentSegment, k].X * AdductionPointsInSegment[CurrentSegment, k + 1].Y -
                    AdductionPointsInSegment[CurrentSegment, k + 1].X * AdductionPointsInSegment[CurrentSegment, k].Y;
                Double Dist = Math.Sqrt(Math.Pow(A, 2) + Math.Pow(B, 2)) / dist;
                UInt32 Left1 = (UInt32)(AdductionPointsInSegment[CurrentSegment, k].X + Dist) + 1;
                UInt32 Left2 = (UInt32)(AdductionPointsInSegment[CurrentSegment, k].X - Dist) - 1;
                UInt32 Right1 = (UInt32)(AdductionPointsInSegment[CurrentSegment, k].Y + Dist) + 1;
                UInt32 Right2 = (UInt32)(AdductionPointsInSegment[CurrentSegment, k].Y - Dist) - 1;
                for (UInt32 i = Left2; i < Left1; ++i)
                {
                    for (UInt32 j = Right2; j < Right1; ++j)
                    {
                        //if (IntersectionLineAndSquare(A, B, C, i, i + 1, j, j + 1, dist))
                        Point[] points = new Point[4];
                        points[0].X = i; points[0].Y = j;
                        points[1].X = i + 1; points[1].Y = j;
                        points[2].X = i + 1; points[2].Y = j + 1;
                        points[3].X = i; points[3].Y = j + 1;
                        if (IntersectionLineAndSquare(AdductionPointsInSegment[CurrentSegment, k],
                                                      AdductionPointsInSegment[CurrentSegment, k+1], 
                                                      points))
                        {
                            if (!Grid[i, j])
                            {
                                CountOfSquare++;
                            }
                            Grid[i, j] = true;
                        }
                    }
                }
            }
            Grid = null;
            return CountOfSquare;
        }
        private void CopyArraysOfPoints(Point[] FromArray, Point[] ToArray, UInt32 Length, UInt32 StartIndexFrom, UInt32 StartIndexTo)
        {
            UInt32 k = StartIndexTo;
            for (UInt32 i = StartIndexFrom; i < Length; ++i)
            {
                ToArray[k++] = FromArray[i];
            }
        }
        private Double ComputeDistBetweenPoints(Point A, Point B)
        {
            return Math.Sqrt(Math.Pow((B.X - A.X), 2) + Math.Pow((B.Y - A.Y), 2));
        }
        private Double ComputeP(Double DistAB, Double DistBC, Double DistAC)
        {
            return (0.5 * (DistAB + DistBC + DistAC));
        }
        private Double ComputeS(Double p, Double DistAB, Double DistBC, Double DistAC)
        {
            return Math.Sqrt(p * (p - DistAB) * (p - DistBC) * (p - DistAC));
        }
        private Double ComputeDistBetweenPointAndLine(Point A, Point B, Point C)
        {
            Double DistAB = ComputeDistBetweenPoints(A, B);
            Double DistBC = ComputeDistBetweenPoints(B, C);
            Double DistAC = ComputeDistBetweenPoints(A, C);
            Double p = ComputeP(DistAB, DistBC, DistAC);
            Double S = ComputeS(p, DistAB, DistBC, DistAC);
            return (2 * S / DistAB);
        }
        private Point[] SimplificationOfCurve(Point[] initialPoints, UInt32 len, Double H, ref UInt32 CountOfNewPoints)
        {
            Point[] NewPoints = new Point[len];
            Point outermostPoint = initialPoints[0];
            UInt32 outermostNum = 0;
            Double outermostDist = 0;
            if (len > 2)
            {
                for (UInt32 i = 1; i < len - 1; ++i)
                {
                    if (ComputeDistBetweenPointAndLine(initialPoints[0], initialPoints[len - 1], initialPoints[i]) >= outermostDist)
                    {
                        outermostDist = ComputeDistBetweenPointAndLine(initialPoints[0], initialPoints[len - 1], initialPoints[i]);
                        outermostNum = i;
                        outermostPoint = initialPoints[i];
                    }
                }
                if (outermostDist < H)
                {
                    NewPoints[0] = initialPoints[0];
                    NewPoints[1] = initialPoints[outermostNum];
                    NewPoints[2] = initialPoints[len - 1];
                    CountOfNewPoints = 3;
                    //for (UInt32 i = 0; i < CountOfNewPoints; ++i)
                    //{
                    //    System.Windows.Forms.MessageBox.Show(NewPoints[i].X.ToString() + " " + NewPoints[i].Y.ToString());
                    //}
                }
                else
                {
                    Point[] InitPoints1 = new Point[outermostNum + 1];
                    Point[] InitPoints2 = new Point[len - outermostNum];
                    UInt32 k = 0;
                    for (UInt32 i = 0; i < outermostNum + 1; ++i)
                    {
                        InitPoints1[k] = initialPoints[i];
                        if (InitPoints1[k].X == 0)
                        {
                            System.Windows.Forms.MessageBox.Show("ERRROOOORRRR");
                            //Point[] point = null;
                            //point[10].X = 0;
                        }
                        k++;
                    }
                    k = 0;
                    for (UInt32 i = outermostNum; i < len; ++i)
                    {
                        InitPoints2[k] = initialPoints[i];
                        if (InitPoints2[k].X == 0)
                        {
                            System.Windows.Forms.MessageBox.Show("ERRROOOORRRR");
                            //Point[] point = null;
                            //point[10].X = 0;
                        }
                        k++;
                    }
                    //CopyArraysOfPoints(initialPoints, InitPoints1, outermostNum + 1, 0, 0);
                    //CopyArraysOfPoints(initialPoints, InitPoints2, len - outermostNum, outermostNum, 0);

                    UInt32 CountOfNewPoints1 = 0;
                    Point[] NewPoints1 = SimplificationOfCurve(InitPoints1, outermostNum + 1, H, ref CountOfNewPoints1);
                    UInt32 CountOfNewPoints2 = 0;
                    Point[] NewPoints2 = SimplificationOfCurve(InitPoints2, len - outermostNum, H, ref CountOfNewPoints2);

                    k = 0;
                    for (UInt32 i = 0; i < CountOfNewPoints1 - 1; ++i)
                    {
                        NewPoints[k++] = NewPoints1[i];
                    }
                    for (UInt32 i = 0; i < CountOfNewPoints2; ++i)
                    {
                        NewPoints[k++] = NewPoints2[i];
                    }
                    CountOfNewPoints = CountOfNewPoints1 - 1 + CountOfNewPoints2;
                }
            }
            else
            {
                NewPoints[0] = initialPoints[0];
                NewPoints[len - 1] = initialPoints[len - 1];

                CountOfNewPoints = 2;
            }

            return NewPoints;
        }
        private Point[] SimplificationOfSegment(Point[,] AdductionPointsInSegment, UInt32[] CountOfAdductionPointsInSegment, UInt32 i, ref UInt32 CountOfPoints)
        {
            UInt32 Len = CountOfAdductionPointsInSegment[i];
            Point[] PointsInSegment = new Point[Len];
            UInt32 k = 0;
            for (UInt32 j = 0; j < Len; ++j)
            {
                PointsInSegment[j] = AdductionPointsInSegment[i, k++];
            }
            Double H = Math.Pow(M, 2 - AngularCoeffRegresLine[i]);

            Point[] NewPoints = SimplificationOfCurve(PointsInSegment, Len, H, ref CountOfPoints);
            return NewPoints;
        }
        public void Simplification()
        {
            UInt32 k = 8;
            UInt32[,] NE = new UInt32[ResultSegmentCount, k];
            Double[,] Dbc = new Double[ResultSegmentCount, k]; // Box-Counting
            for (UInt32 i = 0; i < ResultSegmentCount; ++i)
            {
                for (UInt32 j = 0; j < k; ++j)
                {
                    NE[i, j] = ComputeQuadrics(i, Radius * (j + 1));

                    Dbc[i, j] = Math.Log10(NE[i, j]) / Math.Log10(1 / (Radius * (j + 1)));
                    System.Windows.Forms.MessageBox.Show("NE = " + NE[i, j].ToString());
                    System.Windows.Forms.MessageBox.Show("Dbc = " + Dbc[i, j].ToString());
                }
            }
            Double X = 0;
            Double X2 = 0;
            Double[] Y = new Double[ResultSegmentCount];
            Double[] XY = new Double[ResultSegmentCount];

            Double[] LinearRegK = new Double[ResultSegmentCount];
            Double[] LinearRegB = new Double[ResultSegmentCount];

            AngularCoeffRegresLine = new Double[ResultSegmentCount];

            for (UInt32 i = 0; i < k; ++i)
            {
                X += (Radius * (i + 1));
                X2 += Math.Pow((Radius * (i + 1)), 2);
            }
            X /= 10;
            X2 /= 10;
            for (UInt32 i = 0; i < ResultSegmentCount; ++i)
            {
                for (UInt32 j = 0; j < k; ++j)
                {
                    Y[i] += Dbc[i, j];
                }
                Y[i] /= 10;
                XY[i] = X * Y[i];
                XY[i] /= 10;
            }
            for (UInt32 i = 0; i < ResultSegmentCount; ++i)
            {
                LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * Math.Pow(X, 2));
                LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - Math.Pow(X, 2));
                AngularCoeffRegresLine[i] = LinearRegK[i];
                //System.Windows.Forms.MessageBox.Show(AngularCoeffRegresLine[i].ToString());
            }
            Dbc = null;
            NE = null;
            XY = null;
            Y = null;
            LinearRegB = null;
            LinearRegK = null;

            TotalCountOfPointsAfterSimplification = 0;
            PointsAfterSimplification = new Point[ResultSegmentCount][];
            CountOfPointsAfterSimplification = new UInt32[ResultSegmentCount];
            for (UInt32 i = 0; i < ResultSegmentCount; ++i)
            {
                UInt32 CountOfPoints = 0;
                ///////////System.Windows.Forms.MessageBox.Show(CountOfAdductionPointsInSegment[i].ToString());
                for (UInt32 j = 0; j < CountOfAdductionPointsInSegment[i]; ++j)
                {
                    if (AdductionPointsInSegment[i, j].X == 0)
                    {
                        System.Windows.Forms.MessageBox.Show("Report ERROR");
                        //Point[] point = null;
                        //point[10].X = 100;
                    }
                }
                PointsAfterSimplification[i] = SimplificationOfSegment(AdductionPointsInSegment, CountOfAdductionPointsInSegment, i, ref CountOfPoints);

                //SimplificationOfCurve(AdductionPoints, AdductionCount, 1, ref CountOfPoints);
                CountOfPointsAfterSimplification[i] = CountOfPoints;
                TotalCountOfPointsAfterSimplification += CountOfPointsAfterSimplification[i];
                //System.Windows.Forms.MessageBox.Show("Length 1 = " + CountOfAdductionPointsInSegment[i].ToString() + " 2 = " + CountOfPointsAfterSimplification[i].ToString());
            }
        }

        public void Smoothing()
        {
            CountOfPointsAfterSmoothing = new UInt32[ResultSegmentCount];
            PointsAfterSmoothing = new Point[ResultSegmentCount][];
            TotalCountOfPointsAfterSmoothing = 0;

            for (UInt32 i = 0; i < ResultSegmentCount; ++i)
            {
                CountOfPointsAfterSmoothing[i] = CountOfPointsAfterSimplification[i];
                PointsAfterSmoothing[i] = new Point[CountOfPointsAfterSmoothing[i]];
                TotalCountOfPointsAfterSmoothing += CountOfPointsAfterSmoothing[i];
                PointsAfterSmoothing[i][0].X = PointsAfterSimplification[i][0].X;
                PointsAfterSmoothing[i][0].Y = PointsAfterSimplification[i][0].Y;
                for (UInt32 j = 1; j < CountOfPointsAfterSmoothing[i] - 1; ++j)
                {
                    PointsAfterSmoothing[i][j].X = (PointsAfterSimplification[i][j - 1].X 
                        + 4 * PointsAfterSimplification[i][j].X + PointsAfterSimplification[i][j + 1].X) / 6;
                    PointsAfterSmoothing[i][j].Y = (PointsAfterSimplification[i][j - 1].Y
                        + 4 * PointsAfterSimplification[i][j].Y + PointsAfterSimplification[i][j + 1].Y) / 6;
                }
                PointsAfterSmoothing[i][CountOfPointsAfterSmoothing[i] - 1].X = 
                    PointsAfterSimplification[i][CountOfPointsAfterSimplification[i] - 1].X;
                PointsAfterSmoothing[i][CountOfPointsAfterSmoothing[i] - 1].Y =
                    PointsAfterSimplification[i][CountOfPointsAfterSimplification[i] - 1].Y;
            }
        }
    }
}
