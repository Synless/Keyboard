using System.Collections.Generic;
using System.ComponentModel;

namespace SynlessKeyboardMapper
{
    public class keyboard : INotifyPropertyChanged
    {
        private List<key> keys = new List<key>();
        public List<key> Keys
        {
            get
            {
                return keys;
            }
            set
            {
                keys = value;
                OnPropertyChanged("Keys");      
            }
        }

        public keyboard()
        {
            for(byte n=0;n<29;n++)
            {
                keys.Add(new key(n));
            }
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
