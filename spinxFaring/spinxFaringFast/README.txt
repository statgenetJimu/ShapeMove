# ���R�[�h�Q
���̃R�[�h�Q�� http://www.cs.cmu.edu/~kmcrane/Projects/SpinTransformations/ ��Code C++/Fast�ɂ��ASpin transformation�����Ɋ�Â��A�I�u�W�F�N�g�̋ȗ����ψꉻ����悤�ȕω��ʎw�肷��悤�ɏ������������̂ł�

# �r���h�̎d��
* ���̃v���O������VirtualBox + vagrant + utunbu/trusty �œ���m�F���Ă��܂�

* �ȉ��̃R�}���h��OpenGL��Linear Algebra�A�a�s�񏈗��̃��C�u�������C���X�g�[������K�v������܂�
* SuiteSparse�ɂ��Ă͂����� http://d.hatena.ne.jp/ryamada/20160214 �ɏ�������������܂���

* OpenGL
sudo apt-get install -y mesa-common-dev libglu1-mesa-dev freeglut3-dev

* Linear Algebra
sudo apt-get install -y libsuitesparse-dev

* Boost
sudo apt-get install -y libboost-all-dev

* ���̂����ŁA����prep.txt�Ɠ��K�w�ɂ���Makefile��make���Ă�������
make

# �g����
* Makefile�Ɠ��K�w�ɂł��� spinxFairingFast �Ƃ������s�\�t�@�C���ɂS�̈�����n���܂�
 (1) �O�p�`���b�V���̃I�u�W�F�N�g�t�@�C�� hoge.obj
 (2) ���̎��� �f�t�H���g��-0.95 (-1, 0) �̒l�B�P��̏����ł̋ȗ��̕��R���̒��x���w�肷��
 (3) ���R���B���R�������̉�
 (4) �o�̓t�@�C���̍��ƂȂ镶���� hoge �Ɠ����΁Ahoge0.obj, hoge1.obj,...�Əo��
 
* ���s�R�}���h��
 ./spinxFaringFast ./example/infile/bunny.obj -0.9 10 out

# �쐬obj�t�@�C���̕`�o�́Aviewer�t�H���_�̂�����Q��
