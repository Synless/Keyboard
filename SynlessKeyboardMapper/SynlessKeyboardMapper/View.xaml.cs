using System.Windows;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.KeyDown += new KeyEventHandler(MainWindow_KeyDown);
        }
        void MainWindow_KeyDown(object sender, KeyEventArgs e)
        {            
            Messenger.Default.Send(e.Key.ToString());            
        }
    }
}
