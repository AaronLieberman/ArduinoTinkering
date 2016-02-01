using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Addle.Core.Linq;
using JetBrains.Annotations;
using Managed.Graphics.Direct2D;

namespace FFTVisualizerApp
{
	public partial class FftVisualizerWindow : Form
	{
		readonly Timer _timer1 = new Timer { Interval = 1 };
		Bitmap _cache;
		Direct2DFactory _factory;
		WindowRenderTarget _renderTarget;
		StrokeStyle _strokeStyle;

		public FftVisualizerWindow()
		{
			SetStyle(
				ControlStyles.AllPaintingInWmPaint | ControlStyles.Opaque | ControlStyles.ResizeRedraw | ControlStyles.UserPaint,
				true);
			InitializeComponent();
			Load += MainWindow_Load;
			Paint += MainWindow_Paint;
			_timer1.Tick += timer1_Tick;
		}

		void MainWindow_Paint(object sender, PaintEventArgs e)
		{
			_renderTarget.BeginDraw();
			_renderTarget.Clear(Color.FromKnown(Colors.Black, 1));
			if (_cache != null)
			{
				_renderTarget.DrawBitmap(
					_cache,
					new RectF(0, 0, ClientSize.Width, ClientSize.Height),
					1,
					BitmapInterpolationMode.Linear);
			}
			_renderTarget.EndDraw();
		}

		void MainWindow_Load(object sender, EventArgs e)
		{
			_factory = Direct2DFactory.CreateFactory(FactoryType.SingleThreaded, DebugLevel.None);
			var ssp = new StrokeStyleProperties(
				LineCapStyle.Round,
				LineCapStyle.Round,
				LineCapStyle.Round,
				LineJoin.Round,
				10,
				DashStyle.Solid,
				0);
			_strokeStyle = _factory.CreateStrokeStyle(ssp, null);
			_renderTarget = _factory.CreateWindowRenderTarget(this);
			Resize += MainWindow_Resize;

			_timer1.Start();
		}

		void MainWindow_Resize(object sender, EventArgs e)
		{
			// ReSharper disable once UseNullPropagation
			if (_renderTarget != null)
			{
				//if (this._cache != null)
				//{
				//    this._cache.Dispose();
				//    this._cache = null;
				//}
				_renderTarget.Resize(new SizeU { Width = (uint)ClientSize.Width, Height = (uint)ClientSize.Height });
				//timer1_Tick(null, EventArgs.Empty);
			}
		}

		void timer1_Tick(object sender, EventArgs e)
		{
			_timer1.Enabled = false;
			var rand = new Random();
			_renderTarget.BeginDraw();
			for (var index = 0; index < 20; ++index)
			{
				var color = Color.FromRGB((float)rand.NextDouble(), (float)rand.NextDouble(), (float)rand.NextDouble());
				using (var brush = _renderTarget.CreateSolidColorBrush(color))
				{
					float strokeWidth = rand.Next(1, 5);
					var patch = strokeWidth / 2 - (int)(strokeWidth / 2);
					_renderTarget.DrawRect(
						brush,
						strokeWidth,
						_strokeStyle,
						new RectF(
							new PointF(rand.Next(0, ClientSize.Width) + patch, rand.Next(0, ClientSize.Height) + patch),
							new PointF(rand.Next(0, ClientSize.Width) + patch, rand.Next(0, ClientSize.Height) + patch)));
				}
			}
			_cache = _renderTarget.CreateBitmap(
				new SizeU((uint)ClientSize.Width, (uint)ClientSize.Height),
				IntPtr.Zero,
				0,
				new BitmapProperties(new PixelFormat(DxgiFormat.B8G8R8A8_UNORM, AlphaMode.Ignore), 96, 96));
			_cache.CopyFromRenderTarget(
				new PointU(0, 0),
				_renderTarget,
				new RectU(0, 0, (uint)ClientSize.Width, (uint)ClientSize.Height));
			_renderTarget.EndDraw();
		}
	}
}
