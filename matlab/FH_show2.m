

function FH_show2(im,positions)



for i=1:size(positions,1)
    pos = [positions(i,3) + positions(i,5)/2 positions(i,2) + positions(i,4)/2];
    target_sz = [positions(i,5) positions(i,4)];
    im = insertObjectAnnotation(im, 'rectangle', [pos([2,1]) - target_sz([2,1])/2 target_sz([2,1])],' ','Color','green','TextBoxOpacity',0, 'FontSize', 24);
    textInserter = vision.TextInserter(['ID: ' int2str(positions(i,1))],'Color', [0, 0, 255], 'FontSize', 12, 'Location', [pos(2) pos(1)]);
    im = step(textInserter, im);
end

imshow(im);




