# install python version
python_version=2.7.11

# install packages
sudo apt-get install -y make build-essential libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev

# git clone
git clone https://github.com/yyuu/pyenv.git ~/.pyenv

# set environment variables
echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bash_profile
echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bash_profile

# pyenv initialization
echo 'eval "$(pyenv init -)"' >> ~/.bash_profile

# update shell
source ~/.bash_profile

# install python
pyenv install ${python_version}

# set global python version
pyenv global ${python_version}
