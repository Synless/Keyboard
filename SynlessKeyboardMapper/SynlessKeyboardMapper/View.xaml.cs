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
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            Messenger.Default.Send(e.Key.ToString());
            e.Handled = true;
        }
    }
}
