(use-modules (guix packages)
			 (guix build-system cmake)
			 ((guix licenses) #:prefix license:)
			 (site packages cglm)
			 (gnu packages gl))

(package
  (name "colorbar")
  (version "0.1")
  (source
   (local-file (dirname (current-filename)) #:recursive? #t))
  (build-system cmake-build-system)
  (inputs
   `(("GLEW" ,glew)
	 ("glfw" ,glfw)
	 ("soil" ,soil)
	 ("cglm" ,cglm)))
  (arguments
   `(#:tests? #f
	 #:make-flags
	 (list (string-append "PREFIX=" %output))
	 #:phases
	 (modify-phases %standard-phases
	   (replace 'install
		 (lambda* (#:key outputs #:allow-other-keys)
		   (let* ((out (assoc-ref outputs "out"))
				  (bin (string-append out "/bin")))
			 (install-file "colorbar" bin)
			 #t))))))
  (synopsis "")
  (home-page "")
  (description "")
  (license license:expat))
