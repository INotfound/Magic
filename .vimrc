set nocompatible
filetype on
syntax on

set number
set showcmd
set cursorline
set tabstop=4
set shiftwidth=4
"set updatetime=100

set background=dark
hi Comment ctermfg =241

"colorscheme solarized
"let g:solarized_termcolors =256

call plug#begin('~/.vim/plugged')

"Plug 'skywind3000/quickmenu.vim'
"
Plug 'Valloric/YouCompleteMe', {'do': 'python3 install.py --clang-completer','for':['c', 'cpp']}

Plug 'junegunn/vim-easy-align'

Plug 'scrooloose/nerdtree',{ 'on' : 'NERDTreeToggle'}

Plug 'vim-airline/vim-airline'

Plug 'vim-airline/vim-airline-themes'

Plug 'jiangmiao/auto-pairs'

Plug 'rhysd/vim-clang-format'

call plug#end()

let g:ycm_global_ycm_extra_conf='~/.vim/plugged/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py'

"NERDTree
map <f2> :NERDTreeToggle<cr>
let g:NERDTreeDirArrowExpandable = '+'
let g:NERDTreeDirArrowCollapsible = '-'
let g:NERDTreeWinPos='left'
let g:NERDTreeSize=30
let g:NERDTreeShowLineNumbers=1
let g:NERDTreeHidden=0
