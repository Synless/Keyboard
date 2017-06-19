using System.ComponentModel;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    public class ViewModel : INotifyPropertyChanged
    {

        private keyboard keyboard = new keyboard();

        public keyboard Keyboard
        {
            get
            {
                return keyboard;
            }
            set
            {
                keyboard = value;
                OnPropertyChanged("Keyboard");
            }
        }

        public ICommand Apply;

        public ViewModel()
        {
            Keyboard = keyboard;
            Apply = new Command(pushedApply);
            
        }
        private void pushedApply(object paramter)
        {

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
