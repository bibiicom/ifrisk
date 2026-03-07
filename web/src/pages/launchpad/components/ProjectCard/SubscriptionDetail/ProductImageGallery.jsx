import React, { useState, useEffect } from 'react';
import styles from './ProductImageGallery.module.less';
import image1 from "@/assets/img/launchpad/1.webp";
import image2 from "@/assets/img/launchpad/2.webp";

// 在组件外部定义图片数组，避免重新创建
const images = [image1, image2];

const ProductImageGallery = () => {
  const [currentIndex, setCurrentIndex] = useState(0);

  // 自动播放
  useEffect(() => {
    if (images.length <= 1) return;

    const timer = setInterval(() => {
      setCurrentIndex((prevIndex) => (prevIndex + 1) % images.length);
    }, 5000); // 每5秒切换一次

    return () => clearInterval(timer);
  }, []);

  const handleDotClick = (index) => {
    setCurrentIndex(index);
  };

  return (
    <div className={styles.galleryWrapper}>
      {/* Main Image Display */}
      <div className={styles.mainImageContainer}>
        {images.map((src, index) => (
          <img 
            key={index}
            src={src} 
            alt="Product" 
            className={styles.mainImage}
            style={{ display: currentIndex === index ? 'block' : 'none' }}
          />
        ))}
      </div>

      {/* Dot Indicators */}
      <div className={styles.dotIndicators}>
        {images.map((_, index) => (
          <button
            key={index}
            className={`${styles.dot} ${currentIndex === index ? styles.activeDot : ''}`}
            onClick={() => handleDotClick(index)}
            aria-label={`View image ${index + 1}`}
          />
        ))}
      </div>
    </div>
  );
};

export default React.memo(ProductImageGallery);
