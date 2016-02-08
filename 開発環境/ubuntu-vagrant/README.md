# 使い方
* 準備
  * VirtualBoxとvagrantをインストールする
  * GuestAdditionsの管理はプラグインで行う``vagrant plugin install vagrant-vbguest``
* Boxの準備
  * hashicorpのアカウントを作っておく
  * ``vagrant login``
  * ``vagrant box add ubuntu/trusty64``
  * ``vagrant up --provision``
  * セットアップにしばらく時間がかかるが待つ
* 使う
  * コマンドの実行が終わったら、VirtualBoxのウィンドウでアカウント名vagrant、パスワードvagrantでログインする
  * ``startx``
* 終了と起動
  * シャットダウン：``vagrant halt``
  * 起動：``vagrant up``
  * 一時停止： ``vagrant suspend``
  * 再開: ``vagrant resume``
