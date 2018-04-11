import 'simplegrid/simple-grid.scss'
import './style.less'

import Slideshow from './slideshow'
import Navigation from './navigation'

const ourSlideshow = new Slideshow('.slideshow')
const navi = new Navigation('.nav', ourSlideshow)

window.slideshow = ourSlideshow
