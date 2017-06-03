namespace AutoBat
{
    partial class MainForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.batPathTextBox = new System.Windows.Forms.TextBox();
            this.refButton = new System.Windows.Forms.Button();
            this.batPathLabel = new System.Windows.Forms.Label();
            this.targetListBox = new System.Windows.Forms.ListBox();
            this.resultListBox = new System.Windows.Forms.ListBox();
            this.submitButton = new System.Windows.Forms.Button();
            this.targetGroupBox = new System.Windows.Forms.GroupBox();
            this.resultGroupBox = new System.Windows.Forms.GroupBox();
            this.shutdownCheckBox = new System.Windows.Forms.CheckBox();
            this.targetGroupBox.SuspendLayout();
            this.resultGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // batPathLabel
            // 
            this.batPathLabel.AutoSize = true;
            this.batPathLabel.Location = new System.Drawing.Point(12, 16);
            this.batPathLabel.Name = "batPathLabel";
            this.batPathLabel.Size = new System.Drawing.Size(65, 12);
            this.batPathLabel.TabIndex = 0;
            this.batPathLabel.Text = "バッチファイル";
            // 
            // batPathTextBox
            // 
            this.batPathTextBox.Location = new System.Drawing.Point(83, 13);
            this.batPathTextBox.Name = "batPathTextBox";
            this.batPathTextBox.Size = new System.Drawing.Size(608, 19);
            this.batPathTextBox.TabIndex = 1;
            // 
            // refButton
            // 
            this.refButton.Location = new System.Drawing.Point(697, 11);
            this.refButton.Name = "refButton";
            this.refButton.Size = new System.Drawing.Size(75, 23);
            this.refButton.TabIndex = 2;
            this.refButton.Text = "参照";
            this.refButton.UseVisualStyleBackColor = true;
            this.refButton.Click += new System.EventHandler(this.ClickPathButton);
            // 
            // targetGroupBox
            // 
            this.targetGroupBox.Controls.Add(this.targetListBox);
            this.targetGroupBox.Location = new System.Drawing.Point(14, 38);
            this.targetGroupBox.Name = "targetGroupBox";
            this.targetGroupBox.Size = new System.Drawing.Size(285, 328);
            this.targetGroupBox.TabIndex = 3;
            this.targetGroupBox.TabStop = false;
            this.targetGroupBox.Text = "バッチに渡すファイルをドラッグアンドドロップ";
            // 
            // targetListBox
            // 
            this.targetListBox.AllowDrop = true;
            this.targetListBox.FormattingEnabled = true;
            this.targetListBox.ItemHeight = 12;
            this.targetListBox.Location = new System.Drawing.Point(6, 18);
            this.targetListBox.Name = "targetListBox";
            this.targetListBox.Size = new System.Drawing.Size(273, 292);
            this.targetListBox.TabIndex = 4;
            this.targetListBox.DragDrop += new System.Windows.Forms.DragEventHandler(this.DragDropTarget);
            this.targetListBox.DragEnter += new System.Windows.Forms.DragEventHandler(this.DragEnterTarget);
            // 
            // resultGroupBox
            // 
            this.resultGroupBox.Controls.Add(this.resultListBox);
            this.resultGroupBox.Location = new System.Drawing.Point(305, 38);
            this.resultGroupBox.Name = "resultGroupBox";
            this.resultGroupBox.Size = new System.Drawing.Size(467, 328);
            this.resultGroupBox.TabIndex = 5;
            this.resultGroupBox.TabStop = false;
            this.resultGroupBox.Text = "バッチ出力結果";
            // 
            // resultListBox
            // 
            this.resultListBox.BackColor = System.Drawing.SystemColors.WindowText;
            this.resultListBox.ForeColor = System.Drawing.SystemColors.Window;
            this.resultListBox.FormattingEnabled = true;
            this.resultListBox.ItemHeight = 12;
            this.resultListBox.Location = new System.Drawing.Point(6, 18);
            this.resultListBox.Name = "resultListBox";
            this.resultListBox.SelectionMode = System.Windows.Forms.SelectionMode.None;
            this.resultListBox.Size = new System.Drawing.Size(455, 292);
            this.resultListBox.TabIndex = 6;
            // 
            // submitButton
            // 
            this.submitButton.Location = new System.Drawing.Point(697, 376);
            this.submitButton.Name = "submitButton";
            this.submitButton.Size = new System.Drawing.Size(75, 23);
            this.submitButton.TabIndex = 7;
            this.submitButton.Text = "実行";
            this.submitButton.UseVisualStyleBackColor = true;
            this.submitButton.Click += new System.EventHandler(this.ClickSubmitButton);
            // 
            // shutdownCheckBox
            // 
            this.shutdownCheckBox.AutoSize = true;
            this.shutdownCheckBox.Location = new System.Drawing.Point(14, 380);
            this.shutdownCheckBox.Name = "shutdownCheckBox";
            this.shutdownCheckBox.Size = new System.Drawing.Size(148, 16);
            this.shutdownCheckBox.TabIndex = 8;
            this.shutdownCheckBox.Text = "終了後にシャットダウンする";
            this.shutdownCheckBox.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 411);
            this.Controls.Add(this.shutdownCheckBox);
            this.Controls.Add(this.submitButton);
            this.Controls.Add(this.batPathLabel);
            this.Controls.Add(this.refButton);
            this.Controls.Add(this.batPathTextBox);
            this.Controls.Add(this.targetGroupBox);
            this.Controls.Add(this.resultGroupBox);
            this.Name = "MainForm";
            this.Text = "AutoBat";
            this.targetGroupBox.ResumeLayout(false);
            this.resultGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.TextBox batPathTextBox;
        private System.Windows.Forms.Button refButton;
        private System.Windows.Forms.Label batPathLabel;
        private System.Windows.Forms.ListBox targetListBox;
        private System.Windows.Forms.ListBox resultListBox;
        private System.Windows.Forms.Button submitButton;
        private System.Windows.Forms.GroupBox targetGroupBox;
        private System.Windows.Forms.GroupBox resultGroupBox;
        private System.Windows.Forms.CheckBox shutdownCheckBox;
    }
}

