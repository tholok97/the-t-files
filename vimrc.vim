" Laget av: Thomas Løkkeborg


"------------------------------------- PRE ----------------------------------{{{

" valg for oppførsel av vimrc
let g:msbuild_project = 0           " prosjektnummer (0: OOProg, 1: Asteroids)
let g:spaces_in_a_tab = 4

" set msbuild-greier basert på hvilket prosjekt som jobbes med
let g:msbuild_build_target = ""     " hvor ligger VS prosjektet
let g:msbuild_run_target = ""       " hvor ligger VS programmet

let g:msbuild_build_targets = ["..\\Prosjekt.vcxproj", "Asteroids.vcxproj"]
let g:msbuild_run_targets = ["..\\Debug\\Prosjekt.exe", "Debug\\Asteroids.exe"]
let g:msbuild_mains = [
    \"D:\\Skole\\Objektorintert_Programmering\\Prosjekt\\gruppe26\\main.cpp",
    \"D:\\Skole\\matematikk_for_programmering\\visual\ studio\\Asteroids\\"
            \. "Asteroids\\main.cpp",
\]

" netwr greier. skrur av banneret, skrur på tre, setter bredden på ø-vinduet og
" ignorerer swp-filer 
let g:netrw_banner = 0
let g:netrw_list_hide= '.*\.swp$'

" skrur av highlighting av forrige søk
nohlsearch

" skrur på filtype
filetype on

" sett opp (gui) font-greier om det ikke allerede er satt opp
if !exists("g:font_size")
    let g:font_size = 10
    silent execute "normal! :set guifont=Consolas:h" . font_size . "\<cr>"
endif

"}}}
"-------------------------------- ABBREVIATIONS -----------------------------{{{

iabbrev @@ thomahl@stud.ntnu.no
iabbrev intmain #include <iostream><cr><cr>using namespace std;<cr><cr>int
        \ main() {<cr><cr><cr><cr>return 0;<cr>}

"}}}
"--------------------------------- FUNKSJONER -------------------------------{{{

" funksjoner som erstatter ord under cursor med ord gitt ved input
function! SearchAndReplaceCword()
    let l:word = "\\<" . expand("<cword>") . "\\>"
    let l:replacement = input("Erstatning? ")
    execute ":%s/" . l:word . "/" . l:replacement . "/g"
    normal 
endfunction

" (denne kjører med gc, fra current line)
function! SearchAndReplaceCwordC()
    let l:word = "\\<" . expand("<cword>") . "\\>"
    let l:replacement = input("Erstatning? ")
    execute "%s/" . l:word . "/" . l:replacement . "/gc"
    normal 
endfunction

" søk etter noe gitt ved en motion med vimgrep. Søker igjennom .cpp, .py, .pdb,
" og .h -filer i samme mappe
function! s:GrepOperator(type)
    let g:quickfix_return_to_window = winnr()
    let g:quickfix_is_open = 1
    let l:saved_unnamed_register = @@

    if a:type ==# 'v'
        normal! `<v`>y
    elseif a:type ==# 'char'
        normal! `[v`]y
    else
        return
    endif

    echom "Søker etter: " . @@
    silent execute "vimgrep /" . @@ . "/j *.cpp *.h *.py *.pdb *.md"
    copen

    let @@ = l:saved_unnamed_register
endfunction

" lager tittel basert på kommenteringsstil (param) og input fra bruker
function! MakeTitle(cmt)
    let l:title = input("Tittel? ")
    let l:total = 80 - len(title) - len(a:cmt) - 2
    let l:extra = 0
    if (len(l:title) + len(a:cmt)) % 2 ==# 1
        let l:extra = 1
    endif
    execute "normal! o\<esc>o\<esc>ko" . a:cmt . repeat('-', total/2) . " " .
            \toupper(l:title) . " " . repeat('-', total/2 + l:extra) . "\<esc>"
endfunction

" returnerer x 923'er. x er antall som kan passe fint i statuslinja
function! NiTjueTre()
    return ' ' . repeat('923 ', winwidth(0)/8)
endfunction

" øk (gui) font-størrelsen. (minsk ved negativ parameter)
function! IncGUIFontSize(how_much_to_change_size)
    let g:font_size += a:how_much_to_change_size
    execute "normal! :set guifont=Consolas:h" . g:font_size . "\<cr>"
endfunction

" toggle markdown-modus
let g:is_in_md_mode = 0
function! ToggleMarkdownMode()
    if (g:is_in_md_mode == 0)
        set wrap
        set linebreak
        set nonumber
        set laststatus=0
        let g:is_in_md_mode = 1
    else
        set nowrap
        set nolinebreak
        set number
        set laststatus=2
        let g:is_in_md_mode = 0
    endif
endfunction

" bygg [og kjør] msbuild-prosjektet
function! MakeVCProj(should_run_target_after_success)
    silent execute "normal! :wa\<cr>:silent make! " . g:msbuild_build_target .
            \ "\<cr>"
    if len(getqflist()) == 0
        echom "Suksess!"
        if a:should_run_target_after_success
            call RunMSBuildTarget()
        endif
    else
        echom "Epic Fail.."
    endif
    
endfunction

" toggle quickfix vinduet (fra internett)
function! QuickfixToggle() 
    for i in range(1, winnr('$')) 
        let bnum = winbufnr(i) 
        if getbufvar(bnum, '&buftype') == 'quickfix' 
            cclose 
            return 
        endif 
    endfor 

    copen 
endfunction 

" kjør ms-prosjekt
function! RunMSBuildTarget()
    execute "silent normal! :!" . g:msbuild_run_target . "\<cr>"
endfunction

" åpne matchende cpp/h fil horisontalt eller vertikalt
function! OpenMatchingFile(is_vertical)
    if a:is_vertical
      let l:split_type = "vsp"  
    else
      let l:split_type = "sp"  
    endif
    if expand("%:e") == "h"
        execute "normal! :rightb " . l:split_type . " " . expand("%:r") . 
                \".cpp\<cr>" 
    elseif expand("%:e") == "cpp"
        execute "normal! :rightb " . l:split_type . " " . expand("%:r") . 
                \".h\<cr>" 
    else
        echoerr "OpenMatchingFile error: Ikke .h/.cpp fil!"
    endif
endfunction

function! UpdateBuildStuff(param_project_number)
    let g:msbuild_build_target = g:msbuild_build_targets[a:param_project_number]
    let g:msbuild_run_target = g:msbuild_run_targets[a:param_project_number]
endfunction

function! OpenProjectMain(param_project_number)
    echom "Åpner prosjekt " . a:param_project_number
    execute "silent normal! :e " . g:msbuild_mains[a:param_project_number] . "\<cr>"
    let g:msbuild_project = a:param_project_number
    call UpdateBuildStuff(g:msbuild_project)
endfunction

"}}}
"------------------------------- SETTER VERDIER -----------------------------{{{

" setter opp msbuild
set makeprg=msbuild\ /nologo\ /v:q\ /property:GenerateFullPaths=true
        \\ /nowarn:C4996,C4018,C4101\ /p:configuration=debug

" set tabgreier basert på PRE-variabel
let &tabstop=g:spaces_in_a_tab
let &softtabstop=g:spaces_in_a_tab
let &shiftwidth=g:spaces_in_a_tab

" tabs blir spaces
set expandtab

" div
set number
set backspace=2
set cc=80
set scrolloff=10
set encoding=utf-8
set smartcase
set ignorecase
set autochdir
set wildmenu
"set hlsearch
set incsearch
set cursorline

" cindent for cpp filer 
augroup cpp_specific
    autocmd Filetype cpp setlocal cindent
augroup END

"}}}
"---------------------- KOSMETISK (syntax, highlighting) --------------------{{{

syntax on

augroup stort_vindu
    autocmd!
    autocmd GUIEnter * simalt ~m
augroup END

augroup colorscheme_greier
    autocmd!

    " colorcolumn
    autocmd ColorScheme * highlight ColorColumn ctermbg=6 guibg=darkRed

    " utgangsfarga på statuslinja
    autocmd ColorScheme * highlight ntt ctermbg=0 guibg=Black ctermfg=2
            \ guifg=darkgreen
    autocmd ColorScheme * highlight StatusLine ctermfg=0 guifg=Black ctermbg=2
            \ guibg=darkgreen

    " cursorline greier. EKSPERIMENTELT
    autocmd ColorScheme * highlight clear CursorLine
    autocmd ColorScheme * highlight CursorLine gui=underline ctermbg=black
    autocmd ColorScheme * highlight CursorLineNR gui=bold,underline
            \ ctermbg=black
            \ cterm=bold,underline

    " gui spesifikke greier
    if has("gui_running")
        autocmd ColorScheme * highlight Normal guibg=Black
        autocmd ColorScheme * highlight Visual guifg=Red guibg=Black
        autocmd ColorScheme * highlight Comment guifg=darkred
        autocmd ColorScheme * highlight ColorColumn guibg=Blue
    endif
augroup END

" setter colorscheme
colorscheme darkblue
" setter at statuslinja alltid skal vises
set laststatus=2

" farge på statuslinja skifter ved modusskifte, og vimrc folding
if version >= 700
    augroup statusline_group
        autocmd!
        autocmd InsertEnter * highlight StatusLine ctermfg=0 guifg=Black
                \ ctermbg=5 guibg=purple
        autocmd InsertEnter * highlight ntt ctermbg=0 guibg=Black ctermfg=5
                \ guifg=purple
        autocmd InsertLeave * highlight StatusLine ctermfg=0 guifg=Black
                \ ctermbg=2 guibg=darkgreen
        autocmd InsertLeave * highlight ntt ctermbg=0 guibg=Black ctermfg=2
                \ guifg=darkgreen
        autocmd CursorHold * echo "Whatareyouwaitingfoooooooooooooooooooooooor!"
    autocmd FileType vim setlocal foldmethod=marker
        autocmd Filetype vim setlocal foldlevelstart=0
        autocmd FileType vim setlocal foldcolumn=2
    augroup END
endif

" instillinger for wildmenu
set wildmenu
set wildmode=longest,list
set wildignore+=*.a,*.o,*.exe
set wildignore+=*.bmp,*.gif,*.ico,*.jpg,*.png,*.pdf
set wildignore+=.DS_Store,.git,.hg,.svn
set wildignore+=*~,*.swp,*.tmp

" bygger statuslinja (venstre side)
set statusline=%#Statusline#\  " så uvalgte vinduer ikke blir hvite
set statusline+=%m              " vis om filen er endret
set statusline+=%f              " stien til filen
set statusline+=\ -\            " separator
set statusline+=FileType:       " tekst
set statusline+=%y              " FileTypen til filen
set statusline+=\ \ \ %#ntt#    " spacing og start ny highlight-gruppe  (923)
set statusline+=%{NiTjueTre()}  " skriv ut 923 via funksjon
set statusline+=%#Statusline#   " endre tilbake til default

" (høyre side)
set statusline+=%=              " hopp til andre siden
set statusline+=%4l             " linja vi er på
set statusline+=/               " /
set statusline+=%-4L            " totalt antall linjer

"}}}
"----------------------------------- MAPPING --------------------------------{{{

" space som leder
let mapleader="\<space>"

" endre størrelse på vindu med piltaster
nnoremap <Up>       <C-W>+
nnoremap <Down>     <C-W>-
nnoremap <Left>     <C-W><
nnoremap <Right>    <C-W>>

" Function key -kommandoer (F1: cmd, F2: vimrc, F3: source vimrc, F4: md-mode)
nnoremap <F1>     :call ToggleMarkdownMode()<cr>
nnoremap <F2>     :sp $myvimrc<cr>
nnoremap <F3>     :so $myvimrc<cr>
nnoremap <F4>     :silent !start cmd<cr>
nnoremap <F5>     :silent !explorer .<cr>

" prosjektåner
nnoremap <F12>    :call OpenProjectMain(0)<cr>
nnoremap <F11>    :call OpenProjectMain(1)<cr>

" div. modifikasjoner til bevegelser i vim
nnoremap j gj
nnoremap k gk
inoremap jk <esc>
nnoremap Y y$
"nnoremap H _
"nnoremap L $
nnoremap <c-e> <c-e>j
nnoremap <c-y> <c-y>k

" skifte tab
nnoremap <leader><cr> gt
nnoremap <leader><leader><cr> :tabm +1<cr>
nnoremap <leader><tab> gT
nnoremap <leader><leader><tab> :tabm -1<cr>
nnoremap <leader><bs> :$tabnew 
nnoremap <leader><leader><bs> :$tabnew<cr>:call QuickfixToggle()<cr><cr>

" øk/minske (gui) font-størrelsen
nnoremap <leader><up>       :call IncGUIFontSize(+1)<cr>
nnoremap <leader><down>     :call IncGUIFontSize(-1)<cr>

" vindu-greier:
nnoremap <leader>= <c-w>=
nnoremap <leader>h <c-w>h
nnoremap <leader>j <c-w>j
nnoremap <leader>k <c-w>k
nnoremap <leader>l <c-w>l
nnoremap <leader>H <c-w>H
nnoremap <leader>J <c-w>J
nnoremap <leader>K <c-w>K
nnoremap <leader>L <c-w>L
nnoremap <leader>w <c-w>o
nnoremap <leader><leader>w :tabonly<cr>
nnoremap <leader>q <c-w>q
nnoremap <leader><leader>q :tabclose<cr>
nnoremap <leader>f :sp 
nnoremap <leader>F :sp<cr>
nnoremap <leader><leader>f :call OpenMatchingFile(0)<cr>
nnoremap <leader>d :rightb vsp 
nnoremap <leader>D :rightb vsp<cr>
nnoremap <leader><leader>d :call OpenMatchingFile(1)<cr>
nnoremap <leader>t <c-w>r
nnoremap <leader>T <c-w>T

" søke-greier:
nnoremap <leader>r :call SearchAndReplaceCword() <cr>
nnoremap <leader>e :call SearchAndReplaceCwordC() <cr>
nnoremap <leader>y :set hlsearch<cr>
nnoremap <leader>Y :set nohlsearch<cr>

" mark. 2 stykk. Trykk én gang for å sette, og to for å gå til
nnoremap <leader><leader>s ma
nnoremap <leader>s `a
nnoremap <leader><leader>a mb
nnoremap <leader>a `b

" vimgrep greier:
nnoremap <leader>g :set operatorfunc=<SID>GrepOperator<cr>g@
vnoremap <leader>g :<c-u>call <SID>GrepOperator(visualmode())<cr>
nnoremap <leader>m :call QuickfixToggle()<cr>
nnoremap <leader>n :cnext<cr>
nnoremap <leader>p :cprevious<cr>

" ny linje under/over uten å gå inn i insert-mode
nnoremap <leader>o o<esc>
nnoremap <leader><s-o> O<esc>

" msbuild-greier (ø: bygg(-og-kjør), Ø: bygg, <leader>ø: kjør)
nnoremap <silent> ø              :call MakeVCProj(1)<cr>
nnoremap <silent> Ø              :call MakeVCProj(0)<cr>
nnoremap <silent> <leader>ø      :call RunMSBuildTarget()<cr>

" vis/gjem trailing whitespace
nnoremap <leader>u :match Error /\v\s+$/<cr>
nnoremap <leader>i :match none<cr>

" flytt linje én opp, én ned
nnoremap <leader>+ ddp
nnoremap <leader>- ddkP

" omring ord i hermetegn
nnoremap <leader>2 mqviw<esc>a"<esc>bi"<esc>`ql
nnoremap <leader>" mqF"xf"x`qh
nnoremap <leader>' mqviw<esc>a'<esc>bi'<esc>`ql
nnoremap <leader>* mqF'xf'x`qh

" tre ganger = ingenting
nnoremap <leader><leader><leader> <nop>

" kommenteringssnarveier og qf j,k fix
"...{{{
augroup filetype_specific_stuff
    autocmd!
    autocmd FileType qf nnoremap <buffer> <bs> <C-W><cr>:call
            \ QuickfixToggle()<cr><C-W>T:call QuickfixToggle()<cr>
    autocmd FileType cpp nnoremap <buffer> <leader>b :call
            \ MakeTitle("//")<cr>
    autocmd FileType cpp nnoremap <buffer> <leader>c        mq0i//<esc>`qll
    autocmd FileType cpp nnoremap <buffer> <leader>x        mq0xx`qhh
    autocmd FileType cpp vnoremap <buffer> <leader>c        omq0<S-i>//<esc>`qll
    autocmd FileType cpp vnoremap <buffer> <leader>x        omq0o0lx`qhh
    autocmd FileType python nnoremap <buffer> <leader>b :call
            \ MakeTitle("#")<cr>
    autocmd FileType python nnoremap <buffer> <leader>c     mq0i#<esc>`ql
    autocmd FileType python nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType python vnoremap <buffer> <leader>c     omq0<S-i>#<esc>`ql
    autocmd FileType python vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType vim nnoremap <buffer> <leader>b :call
            \ MakeTitle("\"")<cr>
    autocmd FileType vim nnoremap <buffer> <leader>c        mq0i"<esc>`ql
    autocmd FileType vim nnoremap <buffer> <leader>x        mq0x`qh
    autocmd FileType vim vnoremap <buffer> <leader>c        omq0<S-i>"<esc>`ql
    autocmd FileType vim vnoremap <buffer> <leader>x        omq0o0x`qh
    autocmd FileType prolog nnoremap <buffer> <leader>b :call
            \ MakeTitle("%")<cr>
    autocmd FileType prolog nnoremap <buffer> <leader>c     mq0i%<esc>`ql
    autocmd FileType prolog nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType prolog vnoremap <buffer> <leader>c     omq0<S-i>%<esc>`ql
    autocmd FileType prolog vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType tex nnoremap <buffer> <leader>b :call
            \ MakeTitle("%")<cr>
    autocmd FileType tex nnoremap <buffer> <leader>c        mq0i%<esc>`ql
    autocmd FileType tex nnoremap <buffer> <leader>x        mq0x`qh
    autocmd FileType tex vnoremap <buffer> <leader>c        omq0<S-i>%<esc>`ql
    autocmd FileType tex vnoremap <buffer> <leader>x        omq0o0x`qh
    autocmd FileType qf nnoremap <buffer> j j
    autocmd FileType qf nnoremap <buffer> k k
augroup END
"}}}

"}}}
"------------------------------------ POST ----------------------------------{{{

call UpdateBuildStuff(g:msbuild_project)

"}}}
