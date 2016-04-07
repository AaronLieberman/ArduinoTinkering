using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Addle.Core.Linq;
using JetBrains.Annotations;
using Managed.Graphics.Direct2D;
using Timer = System.Windows.Forms.Timer;

namespace FFTVisualizerApp
{
	public partial class FftVisualizerWindow : Form
	{
		Bitmap _cache;
		Direct2DFactory _factory;
		WindowRenderTarget _renderTarget;
		readonly CancellationTokenSource _cancel = new CancellationTokenSource();
		readonly Timer _frameTimer = new Timer { Interval = 16 };
		Task _readSerialPortTask;
		byte[] _currentData;
		byte[] _averagedData;

		public FftVisualizerWindow()
		{
			SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.Opaque | ControlStyles.ResizeRedraw | ControlStyles.UserPaint, true);
			InitializeComponent();
			Load += MainWindow_Load;
			Paint += MainWindow_Paint;
		}

		protected override void OnClosed(EventArgs e)
		{
			_cancel.Cancel();
			_readSerialPortTask?.Wait();
			base.OnClosed(e);
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
			_renderTarget = _factory.CreateWindowRenderTarget(this);
			Resize += MainWindow_Resize;

			_frameTimer.Tick += (_, __) => Redraw();
		}

		void MainWindow_Resize(object sender, EventArgs e)
		{
			_cache?.Dispose();
			_cache = null;

			_renderTarget?.Resize(new SizeU { Width = (uint)ClientSize.Width, Height = (uint)ClientSize.Height });
			Redraw();
		}

		void Redraw()
		{
			if (_renderTarget == null) return;

			var data = _currentData;
			if (_currentData == null) return;

			if (_averagedData == null || _averagedData.Length != data.Length)
			{
				_averagedData = new byte[data.Length];
			}

			var degrade = 0.2f;

			for (var i = 0; i < data.Length; i++)
			{
				_averagedData[i] = (byte)(_averagedData[i] * (1.0f - degrade) + data[i] * degrade);
			}

			data = _averagedData;

			var rand = new Random();
			_renderTarget.BeginDraw();

			var width = ClientSize.Width;
			var height = ClientSize.Height;

			var dataWidth = data.Length / 4;

			var blockWidth = width / dataWidth;
			var blockHeight = height / 256;

			using (var blackBrush = _renderTarget.CreateSolidColorBrush(Color.FromRGB(0, 0, 0)))
			{
				_renderTarget.FillRect(blackBrush, new RectF(new PointF(0, 0), new PointF(width, height)));
			}

			for (var i = 0; i < dataWidth; i++)
			{
				var color = Color.FromRGB((float)rand.NextDouble(), (float)rand.NextDouble(), (float)rand.NextDouble());

				using (var brush = _renderTarget.CreateSolidColorBrush(color))
				{
					_renderTarget.FillRect(brush,
						new RectF(
							new PointF(i * blockWidth, height - data[i] * blockHeight),
							new PointF((i + 1) * blockWidth, height - (data[i] + 1) * blockHeight)));
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

		void btnConnect_Click(object sender, EventArgs e)
		{
			btnConnect.Visible = false;
			_readSerialPortTask = Task.Run(() => ReadSerialPort(_cancel.Token));
			_frameTimer.Start();
		}

		void ReadSerialPort(CancellationToken token)
		{
			var header = new byte[] { 0xFF, 0xFE, 0xFF, 0xFE };

			var serialPort = new SerialPort("COM3", 115200);
			serialPort.Open();

			while (!token.IsCancellationRequested)
			{
				var buffer = new byte[128];

				var headerIndex = 0;
				while (headerIndex < header.Length)
				{
					var value = serialPort.ReadByte();

					if (value == header[headerIndex])
					{
						headerIndex++;
					}
					else
					{
						headerIndex = 0;
					}
				}

				while (serialPort.BytesToRead < buffer.Length)
				{
					Thread.Yield();
				}

				var count = serialPort.Read(buffer, 0, buffer.Length);
				Debug.Assert(count == 128);

				_currentData = buffer;
			}

			serialPort.Close();
		}
	}
}
