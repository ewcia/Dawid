[%%bs.raw {|
  require('simplegrid/simple-grid.scss')
  require('../../../src/style.less')
|}];

let slides = Slideshow.create(".slideshow");
let withDefaultShown = Slideshow.showImage(slides, 0);
Navigation.initialize(".nav", withDefaultShown);
