import React, { useEffect, useRef } from 'react'
import styles from './AnimatedBackground.module.less'

const AnimatedBackground = () => {
  const canvasRef = useRef(null)
  const animationRef = useRef(null)
  const particlesRef = useRef([])
  const mouseRef = useRef({ x: null, y: null })
  const themeRef = useRef('dark')

  useEffect(() => {
    const canvas = canvasRef.current
    const ctx = canvas.getContext('2d')
    let width, height

    // 获取当前主题
    const getTheme = () => {
      return document.documentElement.getAttribute('data-theme') || 'dark'
    }

    // 主题颜色配置
    const getColors = () => {
      const isDark = themeRef.current !== 'light'
      return {
        // 背景渐变
        bgGradient: isDark
          ? ['#0a0e27', '#0d1229', '#0a0e1f', '#050814']
          : ['#f8fafc', '#f1f5f9', '#e2e8f0', '#ffffff'],
        // 主色调
        primary: isDark ? 'rgba(22, 119, 255,' : 'rgba(22, 119, 255,',
        // 次色调
        secondary: isDark ? 'rgba(99, 102, 241,' : 'rgba(99, 102, 241,',
        // 第三色调
        tertiary: isDark ? 'rgba(139, 92, 246,' : 'rgba(139, 92, 246,',
        // 粒子透明度
        particleOpacity: isDark ? 1 : 0.6,
        // 网格透明度
        gridOpacity: isDark ? 0.03 : 0.05,
        // 光晕透明度
        glowOpacity: isDark ? 1 : 0.5,
      }
    }

    const resize = () => {
      width = canvas.width = window.innerWidth
      height = canvas.height = window.innerHeight
    }

    const createParticles = () => {
      particlesRef.current = []
      const particleCount = Math.floor((width * height) / 15000)

      for (let i = 0; i < particleCount; i++) {
        particlesRef.current.push({
          x: Math.random() * width,
          y: Math.random() * height,
          vx: (Math.random() - 0.5) * 0.5,
          vy: (Math.random() - 0.5) * 0.5,
          radius: Math.random() * 2 + 0.5,
          opacity: Math.random() * 0.5 + 0.2,
          pulseSpeed: Math.random() * 0.02 + 0.01,
          pulseOffset: Math.random() * Math.PI * 2
        })
      }
    }

    const drawGradientBackground = () => {
      const colors = getColors()
      const gradient = ctx.createRadialGradient(
        width * 0.3, height * 0.3, 0,
        width * 0.5, height * 0.5, Math.max(width, height) * 0.8
      )
      gradient.addColorStop(0, colors.bgGradient[0])
      gradient.addColorStop(0.3, colors.bgGradient[1])
      gradient.addColorStop(0.6, colors.bgGradient[2])
      gradient.addColorStop(1, colors.bgGradient[3])

      ctx.fillStyle = gradient
      ctx.fillRect(0, 0, width, height)

      const accentGradient = ctx.createRadialGradient(
        width * 0.8, height * 0.2, 0,
        width * 0.8, height * 0.2, width * 0.4
      )
      accentGradient.addColorStop(0, `${colors.primary} ${0.08 * colors.glowOpacity})`)
      accentGradient.addColorStop(0.5, `${colors.primary} ${0.03 * colors.glowOpacity})`)
      accentGradient.addColorStop(1, 'transparent')

      ctx.fillStyle = accentGradient
      ctx.fillRect(0, 0, width, height)

      const accentGradient2 = ctx.createRadialGradient(
        width * 0.2, height * 0.8, 0,
        width * 0.2, height * 0.8, width * 0.3
      )
      accentGradient2.addColorStop(0, `${colors.secondary} ${0.06 * colors.glowOpacity})`)
      accentGradient2.addColorStop(0.5, `${colors.secondary} ${0.02 * colors.glowOpacity})`)
      accentGradient2.addColorStop(1, 'transparent')

      ctx.fillStyle = accentGradient2
      ctx.fillRect(0, 0, width, height)
    }

    const drawGlowOrbs = (time) => {
      const colors = getColors()
      const orbs = [
        { x: width * 0.15, y: height * 0.25, radius: 300, color: `${colors.primary} ${0.15 * colors.glowOpacity})`, speed: 0.0003 },
        { x: width * 0.85, y: height * 0.75, radius: 250, color: `${colors.secondary} ${0.12 * colors.glowOpacity})`, speed: 0.0004 },
        { x: width * 0.5, y: height * 0.5, radius: 400, color: `${colors.tertiary} ${0.08 * colors.glowOpacity})`, speed: 0.0002 }
      ]

      orbs.forEach(orb => {
        const pulseRadius = orb.radius + Math.sin(time * orb.speed * 1000) * 50
        const gradient = ctx.createRadialGradient(
          orb.x + Math.sin(time * orb.speed * 500) * 50,
          orb.y + Math.cos(time * orb.speed * 500) * 30,
          0,
          orb.x, orb.y,
          pulseRadius
        )
        gradient.addColorStop(0, orb.color)
        gradient.addColorStop(0.5, orb.color.replace('0.15', '0.05').replace('0.12', '0.04').replace('0.08', '0.03'))
        gradient.addColorStop(1, 'transparent')

        ctx.fillStyle = gradient
        ctx.fillRect(0, 0, width, height)
      })
    }

    const drawParticles = (time) => {
      const colors = getColors()
      particlesRef.current.forEach((particle, index) => {
        particle.x += particle.vx
        particle.y += particle.vy

        if (particle.x < 0 || particle.x > width) particle.vx *= -1
        if (particle.y < 0 || particle.y > height) particle.vy *= -1

        const pulse = Math.sin(time * particle.pulseSpeed * 1000 + particle.pulseOffset)
        const currentOpacity = (particle.opacity + pulse * 0.15) * colors.particleOpacity
        const currentRadius = particle.radius + pulse * 0.3

        ctx.beginPath()
        ctx.arc(particle.x, particle.y, currentRadius, 0, Math.PI * 2)
        ctx.fillStyle = `${colors.primary} ${Math.max(0.1, currentOpacity)})`
        ctx.fill()

        if (mouseRef.current.x !== null && mouseRef.current.y !== null) {
          const dx = particle.x - mouseRef.current.x
          const dy = particle.y - mouseRef.current.y
          const distance = Math.sqrt(dx * dx + dy * dy)

          if (distance < 150) {
            const opacity = (1 - distance / 150) * 0.3 * colors.particleOpacity
            ctx.beginPath()
            ctx.moveTo(particle.x, particle.y)
            ctx.lineTo(mouseRef.current.x, mouseRef.current.y)
            ctx.strokeStyle = `${colors.primary} ${opacity})`
            ctx.lineWidth = 0.5
            ctx.stroke()
          }
        }

        for (let j = index + 1; j < particlesRef.current.length; j++) {
          const other = particlesRef.current[j]
          const dx = particle.x - other.x
          const dy = particle.y - other.y
          const distance = Math.sqrt(dx * dx + dy * dy)

          if (distance < 100) {
            const opacity = (1 - distance / 100) * 0.15 * colors.particleOpacity
            ctx.beginPath()
            ctx.moveTo(particle.x, particle.y)
            ctx.lineTo(other.x, other.y)
            ctx.strokeStyle = `${colors.primary} ${opacity})`
            ctx.lineWidth = 0.3
            ctx.stroke()
          }
        }
      })
    }

    const drawGrid = () => {
      const colors = getColors()
      const gridSize = 60
      ctx.strokeStyle = `${colors.primary} ${colors.gridOpacity})`
      ctx.lineWidth = 1

      for (let x = 0; x < width; x += gridSize) {
        ctx.beginPath()
        ctx.moveTo(x, 0)
        ctx.lineTo(x, height)
        ctx.stroke()
      }

      for (let y = 0; y < height; y += gridSize) {
        ctx.beginPath()
        ctx.moveTo(0, y)
        ctx.lineTo(width, y)
        ctx.stroke()
      }
    }

    const animate = (time) => {
      // 检查主题变化
      themeRef.current = getTheme()

      ctx.clearRect(0, 0, width, height)

      drawGradientBackground()
      drawGlowOrbs(time)
      drawGrid()
      drawParticles(time)

      animationRef.current = requestAnimationFrame(animate)
    }

    const handleMouseMove = (e) => {
      mouseRef.current = { x: e.clientX, y: e.clientY }
    }

    const handleMouseLeave = () => {
      mouseRef.current = { x: null, y: null }
    }

    resize()
    createParticles()
    animate(0)

    window.addEventListener('resize', () => {
      resize()
      createParticles()
    })
    window.addEventListener('mousemove', handleMouseMove)
    window.addEventListener('mouseleave', handleMouseLeave)

    return () => {
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current)
      }
      window.removeEventListener('resize', resize)
      window.removeEventListener('mousemove', handleMouseMove)
      window.removeEventListener('mouseleave', handleMouseLeave)
    }
  }, [])

  return (
    <div className={styles.animatedBackground}>
      <canvas ref={canvasRef} className={styles.canvas} />
      <div className={styles.overlay} />
    </div>
  )
}

export default AnimatedBackground
