


ccc;

addpath('./tracker/');


markObject( 1,  214,  173,  27,  32 );
im = imread('./video/1475141139413_img/101.png');
[positions] = feedFrame(  101,  im );FH_show2(im,positions);pause(.5);





markObject( 2,  314,  173,  27,  32 );
im = imread('./video/1475141139413_img/102.png');
[positions] = feedFrame(  102,  im );FH_show2(im,positions);pause(.5);

markObject( 3,  214,  273,  27,  32 );
im = imread('./video/1475141139413_img/103.png');
[positions] = feedFrame(  103,  im );FH_show2(im,positions);pause(.5);

% endMarkObject(1);
im = imread('./video/1475141139413_img/104.png');
[positions] = feedFrame(  102,  im );FH_show2(im,positions);pause(.5);
im = imread('./video/1475141139413_img/105.png');
[positions] = feedFrame(  102,  im );FH_show2(im,positions);pause(.5);

% endMarkObject(2);
im = imread('./video/1475141139413_img/106.png');
[positions] = feedFrame(  102,  im );FH_show2(im,positions);pause(.5);
im = imread('./video/1475141139413_img/107.png');
[positions] = feedFrame(  101,  im );FH_show2(im,positions);pause(.5);



im = imread('./video/1475141139413_img/100.png');   % when frame number is earlier than existing tracklets, they will be deleted 
[positions] = feedFrame(  100,  im );FH_show2(im,positions);pause(.5);
markObject( 1,  214,  173,  27,  32 );
im = imread('./video/1475141139413_img/107.png');
[positions] = feedFrame(  107,  im );FH_show2(im,positions);pause(.5);



for i=108:220
    im = imread(['./video/1475141139413_img/' int2str(i) '.png']);
    [positions] = feedFrame(  i,  im );FH_show2(im,positions);drawnow;
end

