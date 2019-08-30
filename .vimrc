set nocompatible

filetype on

set number

syntax on

set showcmd
set cursorline
set tabstop=4
set shiftwidth=4
"set updatetime=100

set background=dark
"colorscheme solarized
"let g:solarized_termcolors =256
hi Comment ctermfg =241

call plug#begin('~/.vim/plugged')

"Plug 'skywind3000/quickmenu.vim'

Plug 'junegunn/vim-easy-align'

Plug 'scrooloose/nerdtree',{ 'on' : 'NERDTreeToggle'}

Plug 'vim-airline/vim-airline'

Plug 'vim-airline/vim-airline-thremes'

Plug 'jiangmiao/auto-pairs'

Plug 'rhysd/vim-clang-format'

call plug#end()
