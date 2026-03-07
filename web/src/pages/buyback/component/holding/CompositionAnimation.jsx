import React, { useEffect, useRef, useState } from 'react'
import styles from './CompositionAnimation.module.less'

// 缓动函数
const easeOutCubic = (x) => {
  return 1 - Math.pow(1 - x, 3)
}

const CompositionAnimation = ({ imageUrl, visible, onComplete }) => {
  const canvasRef = useRef(null)
  const [skipVisible, setSkipVisible] = useState(false)
  const [isGlowing, setIsGlowing] = useState(false)
  const animationRef = useRef(null)
  const particlesRef = useRef([])
  const imageRef = useRef(null)
  const loadSeqRef = useRef(0)

  // 配置参数
  const PARTICLE_SIZE = 10 // 粒子大小
  const DURATION = 2000 // 动画持续时间 ms

  useEffect(() => {
    if (visible && imageUrl) {
      const seq = ++loadSeqRef.current
      init(seq)
      setIsGlowing(false)
      setSkipVisible(false)
    } else {
      // 清理
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current)
      }
    }
    return () => {
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current)
      }
      loadSeqRef.current++
    }
  }, [visible, imageUrl])

  const loadImage = (src) => {
    return new Promise((resolve, reject) => {
      const img = new Image()
      img.onload = () => resolve(img)
      img.onerror = () => {
        const corsImg = new Image()
        corsImg.crossOrigin = 'Anonymous'
        corsImg.onload = () => resolve(corsImg)
        corsImg.onerror = reject
        corsImg.src = src
      }
      img.src = src
    })
  }

  const init = (seq) => {
    const canvas = canvasRef.current
    if (!canvas) return

    const ctx = canvas.getContext('2d')

    loadImage(imageUrl)
      .then((img) => {
        if (seq !== loadSeqRef.current) return
        imageRef.current = img

        // 设置画布大小
        // 限制最大宽度，保持比例
        const maxWidth = Math.min(window.innerWidth * 0.8, 500)
        const scale = maxWidth / img.width
        const width = img.width * scale
        const height = img.height * scale

        canvas.width = width
        canvas.height = height

        // 生成粒子
        createParticles(img, width, height, scale)

        // 开始动画
        const startTime = Date.now()
        animate(ctx, width, height, startTime)
        setSkipVisible(true)
      })
      .catch(() => {
        if (seq !== loadSeqRef.current) return
        setSkipVisible(true)
      })
  }

  const createParticles = (img, width, height, scale) => {
    const particles = []
    const cols = Math.ceil(width / PARTICLE_SIZE)
    const rows = Math.ceil(height / PARTICLE_SIZE)
    
    // 离屏 Canvas 用于获取像素数据（如果需要颜色处理，暂时直接用drawImage裁剪）
    
    for (let i = 0; i < cols; i++) {
      for (let j = 0; j < rows; j++) {
        const tx = i * PARTICLE_SIZE
        const ty = j * PARTICLE_SIZE
        
        // 随机起始位置 (从屏幕四周飞入)
        const angle = Math.random() * Math.PI * 2
        const distance = Math.max(width, height) * (1.5 + Math.random())
        const sx = width / 2 + Math.cos(angle) * distance
        const sy = height / 2 + Math.sin(angle) * distance

        particles.push({
          sx, // 起始 x
          sy, // 起始 y
          tx, // 目标 x
          ty, // 目标 y
          frameX: i * (PARTICLE_SIZE / scale), // 图片源 x
          frameY: j * (PARTICLE_SIZE / scale), // 图片源 y
          size: PARTICLE_SIZE,
          delay: Math.random() * 500 // 随机延迟，增加层次感
        })
      }
    }
    particlesRef.current = particles
  }

  const animate = (ctx, width, height, startTime) => {
    const now = Date.now()
    const elapsed = now - startTime
    
    // 清空画布
    ctx.clearRect(0, 0, width, height)
    
    let allFinished = true
    
    particlesRef.current.forEach(p => {
      // 计算每个粒子的进度
      // 减去延迟时间
      let progress = (elapsed - p.delay) / DURATION
      if (progress < 0) progress = 0
      if (progress > 1) progress = 1
      
      if (progress < 1) allFinished = false

      const ease = easeOutCubic(progress)
      
      const currentX = p.sx + (p.tx - p.sx) * ease
      const currentY = p.sy + (p.ty - p.sy) * ease
      
      // 绘制粒子
      if (imageRef.current && progress > 0) {
        // 保存上下文
        ctx.save()
        // 移动到粒子位置
        ctx.translate(currentX, currentY)
        
        // 缩放效果：开始小，结束正常
        const scale = 0.1 + 0.9 * ease
        ctx.scale(scale, scale)
        
        // 绘制图片片段
        // drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)
        // 注意：源坐标需要除以 scale (因为我们计算宽高时缩放了，但源图片是原始尺寸)
        const imgScale = width / imageRef.current.width
        
        ctx.drawImage(
          imageRef.current,
          p.frameX, 
          p.frameY, 
          p.size / imgScale, 
          p.size / imgScale,
          0, 
          0, 
          p.size, 
          p.size
        )
        
        ctx.restore()
      }
    })

    if (!allFinished) {
      animationRef.current = requestAnimationFrame(() => animate(ctx, width, height, startTime))
    } else {
      // 动画完成，绘制完整图片以防有缝隙
      ctx.clearRect(0, 0, width, height)
      ctx.drawImage(imageRef.current, 0, 0, width, height)
      
      // 开启发光效果
      setIsGlowing(true)
      
      // 延迟触发完成回调，展示发光效果
      setTimeout(() => {
        if (onComplete) onComplete()
      }, 3000)
    }
  }

  if (!visible) return null

  return (
    <div className={styles['animation-overlay']}>
      <canvas ref={canvasRef} className={isGlowing ? styles.glow : ''} />
      {skipVisible && (
        <button className={styles['skip-btn']} onClick={onComplete}>
         Skip
        </button>
      )}
    </div>
  )
}

export default CompositionAnimation
