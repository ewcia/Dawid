[%%bs.raw {|
  require('simplegrid/simple-grid.scss')
  require('../../../src/style.less')
|}];

let slides = Slideshow.create(".slideshow");
Navigation.initialize(".nav", slides);
let withDefaultShown = Slideshow.showImage(slides, 0);
