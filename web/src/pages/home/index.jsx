import React from 'react'

import Col from '../../component/col'

import AnimatedBackground from './components/AnimatedBackground/AnimatedBackground'
import HeroSection from './components/HeroSection/HeroSection'
import FeaturesSection from './components/FeaturesSection/FeaturesSection'
import CTASection from './components/CTASection/CTASection'

import authHOC from '../common/hoc/authHOC'

import './index.less'

const Home = () => {
  return (
    <Col className="home">
      <AnimatedBackground />
      <HeroSection />
      <FeaturesSection />
      <CTASection />
    </Col>
  )
}

Home.displayName = 'Home'

export default authHOC(true)(Home)
