using System.ComponentModel;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    public class key : INotifyPropertyChanged
    {
        private string keyString = "0";
        public string KeyString
        {
            get
            {
                return keyString;
            }
            set
            {
                keyString = value;
                OnPropertyChanged("KeyString");
            }
        }
        private string keyChar = "-";
        public string KeyChar
        {
            get
            {
                return keyChar;
            }
            set
            {
                keyChar = value;
                
                OnPropertyChanged("KeyChar");
            }
        }

        private bool enabled = true;
        public bool Enabled
        {
            get
            {
                return enabled;
            }
            set
            {
                enabled = value;
                OnPropertyChanged("Enabled");
            }
        }

        public key()
        {
        }
        public key(byte n)
        {
            KeyChar = n.ToString();
        }


        #region PropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }
}