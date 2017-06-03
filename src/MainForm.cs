using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoBat
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ClickPathButton
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ClickPathButton(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog()
            {
                Filter = "BATファイル(*.bat)|*.bat|すべてのファイル(*.*)|*.*",
                FilterIndex = 1,
                Title = "バッチファイルを選択してください",
                RestoreDirectory = true
            };

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                batPathTextBox.Text = ofd.FileName;
            }
        }

        /// <summary>
        /// DragEnterTarget
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DragEnterTarget(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.All;
            }
        }

        /// <summary>
        /// DragDropTarget
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DragDropTarget(object sender, DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                return;
            }
            foreach (var filePath in (string[])e.Data.GetData(DataFormats.FileDrop))
            {
                targetListBox.Items.Add(filePath);
            }
        }

        /// <summary>
        /// ClickSubmitButton
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void ClickSubmitButton(object sender, EventArgs e)
        {
            string bat = batPathTextBox.Text;
            string target = "";
            bool retc = true;

            if (bat == string.Empty || targetListBox.Items.Count == 0)
            {
                return;
            }
            
            submitButton.Enabled = false;

            while (retc == true)
            {
                target = (string)targetListBox.Items[0];
                await Task.Run(() =>
                {
                    ExecuteBat(bat, target);
                });

                targetListBox.Items.RemoveAt(0);
                if (targetListBox.Items.Count == 0)
                {
                    retc = false;
                }
            }
            
            submitButton.Enabled = true;

            if (shutdownCheckBox.Checked)
            {
                ProcessStartInfo psi = new ProcessStartInfo()
                {
                    FileName = "shutdown.exe",
                    Arguments = "/s",
                    UseShellExecute = false,
                    CreateNoWindow = true
                };
                Process p = Process.Start(psi);
            }
        }

        /// <summary>
        /// ExecuteBat
        /// </summary>
        /// <param name="bat">bat file</param>
        /// <param name="target">target file</param>
        private void ExecuteBat(string bat, string target)
        {
            bat = "\"" + bat + "\"";
            target = "\"" + target + "\"";

            Process p = new Process();

            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardInput = true;

            p.StartInfo.RedirectStandardOutput = true;
            p.OutputDataReceived += OutputCmd;

            p.StartInfo.FileName = Environment.GetEnvironmentVariable("ComSpec");
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.Arguments = string.Format(@"/c ""{0} {1}""", bat, target);

            p.Start();
            p.BeginOutputReadLine();
            
            p.WaitForExit();
            p.Close();
        }

        /// <summary>
        /// OutputCmd
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OutputCmd(object sender, DataReceivedEventArgs e)
        {
            Invoke(new Action(() => {
                if (e.Data != null)
                {
                    resultListBox.Items.Add(e.Data);
                    resultListBox.TopIndex = resultListBox.Items.Count - 1;
                }
            }));
        }
    }
}
