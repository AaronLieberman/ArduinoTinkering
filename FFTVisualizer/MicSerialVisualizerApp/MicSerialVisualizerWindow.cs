using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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
		const int _sampleCount = 512;

		Bitmap _cache;
		Direct2DFactory _factory;
		WindowRenderTarget _renderTarget;
		readonly CancellationTokenSource _cancel = new CancellationTokenSource();
		readonly Timer _frameTimer = new Timer { Interval = 16 };
		Task _readSerialPortTask;
		float[] _currentData;
		byte[] _averagedData;

		public FftVisualizerWindow()
		{
			SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.Opaque | ControlStyles.ResizeRedraw | ControlStyles.UserPaint, true);
			InitializeComponent();
			Load += MainWindow_Load;

			pictArea.Paint += PictArea_Paint;
		}

		protected override void OnClosed(EventArgs e)
		{
			_cancel.Cancel();
			_readSerialPortTask?.Wait();
			base.OnClosed(e);
		}

		void PictArea_Paint(object sender, PaintEventArgs e)
		{
			_renderTarget.BeginDraw();

			if (_cache != null)
			{
				_renderTarget.DrawBitmap(
					_cache,
					new RectF(0, 0, pictArea.Width, pictArea.Height),
					1,
					BitmapInterpolationMode.Linear);
			}

			_renderTarget.EndDraw();
		}

		void MainWindow_Load(object sender, EventArgs e)
		{
			_factory = Direct2DFactory.CreateFactory(FactoryType.SingleThreaded, DebugLevel.None);
			_renderTarget = _factory.CreateWindowRenderTarget(pictArea);
			Resize += MainWindow_Resize;

			_frameTimer.Tick += (_, __) => Redraw();
		}

		void MainWindow_Resize(object sender, EventArgs e)
		{
			_cache?.Dispose();
			_cache = null;

			_renderTarget?.Resize(new SizeU { Width = (uint)pictArea.Width, Height = (uint)pictArea.Height });

			Redraw();
			Refresh();
		}

		void Redraw()
		{
			if (_renderTarget == null) return;

			var data = _currentData;
			if (_currentData == null) return;

			//if (_averagedData == null || _averagedData.Length != data.Length)
			//{
			//	_averagedData = new byte[data.Length];
			//}

			//const float degrade = 1f;

			//for (var i = 0; i < data.Length; i++)
			//{
			//	_averagedData[i] = (byte)(_averagedData[i] * (1.0f - degrade) + data[i] * degrade);
			//}

			//for (var i = 0; i < data.Length / 4; i++)
			//{
			//	_averagedData[i * 4] = (byte)(_averagedData[i * 4] * (1.0f - degrade) + data[i * 4] * degrade);
			//	_averagedData[i * 4 + 1] = _averagedData[i * 4];
			//	_averagedData[i * 4 + 2] = _averagedData[i * 4];
			//	_averagedData[i * 4 + 3] = _averagedData[i * 4];
			//}

			//data = _averagedData;

			_renderTarget.BeginDraw();

			var width = pictArea.Width;
			var height = pictArea.Height;

			var dataWidth = data.Length / 2;

			var blockWidth = width / dataWidth;
			var blockHeight = height / 256;

			_renderTarget.Clear(Color.FromKnown(Colors.Black, 1));

			for (var i = 0; i < dataWidth; i++)
			{
				var value = data[i] * 255;
				var color1 = Color.FromRGB(255, 255 - value, 255 - value);

				using (var brush1 = _renderTarget.CreateSolidColorBrush(color1))
				{
					_renderTarget.FillRect(brush1,
						new RectF(
							new PointF(i * blockWidth, height - value * blockHeight),
							new PointF((i + 1) * blockWidth, height - (value + 1) * blockHeight)));
				}
			}

			_cache = _renderTarget.CreateBitmap(
				new SizeU((uint)pictArea.Width, (uint)pictArea.Height),
				IntPtr.Zero,
				0,
				new BitmapProperties(new PixelFormat(DxgiFormat.B8G8R8A8_UNORM, AlphaMode.Ignore), 96, 96));

			_cache.CopyFromRenderTarget(
				new PointU(0, 0),
				_renderTarget,
				new RectU(0, 0, (uint)pictArea.Width, (uint)pictArea.Height));

			_renderTarget.EndDraw();
		}

		void btnConnect_Click(object sender, EventArgs e)
		{
			btnConnect.Visible = false;
			_readSerialPortTask = Task.Run(() => ReadSerialPort(_cancel.Token));
			_frameTimer.Start();

			btnConnect.Enabled = false;
		}

		static void WaitForBytesReady(SerialPort serialPort, int bytes, CancellationToken token)
		{
			while (serialPort.BytesToRead < bytes)
			{
				Thread.Yield();
				token.ThrowIfCancellationRequested();
			}
		}

		static byte[] ReadBytes(SerialPort serialPort, CancellationToken token)
		{
			var buffer = new byte[_sampleCount];
			WaitForBytesReady(serialPort, buffer.Length, token);

			var count = serialPort.Read(buffer, 0, buffer.Length);
			Debug.Assert(count == buffer.Length);

			return buffer;
		}

		void ReadSerialPort(CancellationToken token)
		{
			using (var fs = File.OpenWrite(@"c:\users\aaron\desktop\data.bin"))
			{
				var header = new byte[] { 0xFF, 0xFE, 0xFF, 0xFE };

				var serialPort = new SerialPort("COM3", 115200);
				serialPort.Open();
				try
				{
					while (!token.IsCancellationRequested)
					{
						var headerIndex = 0;

						while (headerIndex < header.Length)
						{
							WaitForBytesReady(serialPort, 1, token);

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

						var bytes = ReadBytes(serialPort, token);
						_currentData = new float[256];
						for (var i = 0; i < 256; i++)
						{
							_currentData[i] = ((float)((bytes[i * 2 + 1] << 8) | bytes[i * 2])) / 1023;
						}

						fs.Write(bytes, 0, bytes.Length);
					}
				}
				catch (OperationCanceledException)
				{
				}
				finally
				{
					serialPort.Close();
				}
			}
		}
	}
}
