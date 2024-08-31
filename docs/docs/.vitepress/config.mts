import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Nautilus AVInfo",
  description: "Documentation for the Nautilus AVInfo extension.",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'User Guide', link: '/' },
    ],

    sidebar: [
      {
        text: 'Reference',
        items: [
          { text: 'User Guide', link: '/' },
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/ezhai/nautilus-avinfo' }
    ]
  }
})
