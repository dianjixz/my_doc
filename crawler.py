#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
import time
import logging

# 设置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class WebCrawler:
    def __init__(self, base_url):
        self.base_url = base_url
        self.session = requests.Session()
        self.visited_urls = set()
        self.found_paths = set()
        
    def get_page_content(self, url):
        """获取页面内容"""
        try:
            response = self.session.get(url, timeout=10)
            response.raise_for_status()
            return response.text
        except requests.RequestException as e:
            logger.error(f"获取页面失败 {url}: {e}")
            return None
    
    def extract_links(self, html_content, base_url):
        """从HTML内容中提取所有链接"""
        links = []
        try:
            soup = BeautifulSoup(html_content, 'html.parser')
            
            # 查找所有的<a>标签
            for link in soup.find_all('a', href=True):
                href = link['href']
                # 处理相对路径和绝对路径
                full_url = urljoin(base_url, href)
                links.append(full_url)
                
            # 查找所有的<link>标签 (CSS等)
            for link in soup.find_all('link', href=True):
                href = link['href']
                full_url = urljoin(base_url, href)
                links.append(full_url)
                
            # 查找所有的<script>标签
            for script in soup.find_all('script', src=True):
                src = script['src']
                full_url = urljoin(base_url, src)
                links.append(full_url)
                
            # 查找所有的<img>标签
            for img in soup.find_all('img', src=True):
                src = img['src']
                full_url = urljoin(base_url, src)
                links.append(full_url)
                
        except Exception as e:
            logger.error(f"解析页面链接失败: {e}")
            
        return links
    
    def is_valid_path(self, url):
        """判断是否为有效的接口文件路径"""
        # 这里可以根据需要调整规则
        # 例如：排除图片、CSS、JS等静态资源
        excluded_extensions = ['.png', '.jpg', '.jpeg', '.gif', '.css', '.js', '.ico', '.svg']
        parsed_url = urlparse(url)
        path = parsed_url.path.lower()
        
        # 检查扩展名
        for ext in excluded_extensions:
            if path.endswith(ext):
                return False
                
        return True
    
    def crawl(self, url):
        """爬取单个页面"""
        if url in self.visited_urls:
            return
            
        logger.info(f"正在爬取: {url}")
        self.visited_urls.add(url)
        
        html_content = self.get_page_content(url)
        if not html_content:
            return
            
        links = self.extract_links(html_content, url)
        
        for link in links:
            # 只处理同域名下的链接
            if urlparse(link).netloc == urlparse(self.base_url).netloc:
                if self.is_valid_path(link):
                    self.found_paths.add(link)
                    # 如果是目录页，继续爬取
                    if not self.is_file_path(link):
                        self.crawl(link)
    
    def is_file_path(self, url):
        """判断是否为文件路径（根据扩展名）"""
        parsed_url = urlparse(url)
        path = parsed_url.path
        # 简单判断是否有扩展名
        return '.' in path.split('/')[-1]
    
    def start_crawling(self):
        """开始爬取"""
        logger.info(f"开始爬取网站: {self.base_url}")
        self.crawl(self.base_url)
        
        # 打印找到的路径
        print("\n=== 找到的接口文件路径 ===")
        for path in sorted(self.found_paths):
            print(path)
        print(f"\n总共找到 {len(self.found_paths)} 个路径")
        
        return self.found_paths

def main():
    base_url = "http://192.168.20.195:6052/"
    
    # 创建爬虫实例
    crawler = WebCrawler(base_url)
    
    try:
        # 开始爬取
        paths = crawler.start_crawling()
        
        # 实时监控模式
        print("\n=== 实时监控模式 ===")
        while True:
            print("按 Ctrl+C 停止监控")
            time.sleep(30)  # 每30秒重新检查一次
            
            # 重新爬取以发现新路径
            new_crawler = WebCrawler(base_url)
            new_paths = new_crawler.start_crawling()
            
            # 检查是否有新路径
            added_paths = new_paths - crawler.found_paths
            if added_paths:
                print("\n=== 新增的路径 ===")
                for path in added_paths:
                    print(path)
                # 更新已知路径
                crawler.found_paths.update(new_paths)
                
    except KeyboardInterrupt:
        print("\n监控已停止")

if __name__ == "__main__":
    main()
