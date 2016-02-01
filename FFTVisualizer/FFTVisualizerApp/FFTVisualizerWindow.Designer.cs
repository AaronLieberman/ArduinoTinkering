namespace FFTVisualizerApp
{
	partial class FftVisualizerWindow
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.SuspendLayout();
			// 
			// FftVisualizerWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(779, 581);
			this.Name = "FftVisualizerWindow";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.MainWindow_Load);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainWindow_Paint);
			this.Resize += new System.EventHandler(this.MainWindow_Resize);
			this.ResumeLayout(false);

		}

		#endregion
	}
}

